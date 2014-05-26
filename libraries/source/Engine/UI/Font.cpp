#include <Engine/UI/Font.hpp>

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include FT_GLYPH_H
#include FT_BITMAP_H

namespace Engine
{
	namespace UI
	{
		Font::Font()
		: m_library(NULL)
		, m_face(NULL)
		{
			// Nothing to do.
		}

		Font::~Font()
		{
			// Nothing to do.
		}

		bool Font::LoadFromFile(std::string filepath)
		{
			// Initialize FreeType.
			FT_Library library;
			if (FT_Init_FreeType(&library) != 0)
			{
				std::cerr << "ERROR: Failed to  initialize FreeType" << std::endl;
				return false;
			}
			m_library = library;

			// Load the font face from the specified file.
			FT_Face face;
			if (FT_New_Face(library, filepath.c_str(), 0, &face) != 0)
			{
				std::cerr << "ERROR: Failed to load font face" << std::endl;
				return false;
			}
			m_face = face;

			// Select the unicode character map for the font face.
			if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
    		{
    			std::cerr << "ERROR: Failed to select font face character map" << std::endl;
				return false;
    		}

    		// Font was loaded successfully!
    		return true;
		}

		std::shared_ptr<Texture> Font::GetTexture(unsigned int characterSize) const
		{
			return m_pages[characterSize].texture;
		}

		const Glyph& Font::GetGlyph(unsigned int codePoint, unsigned int characterSize) const
		{
			// Get the table of glyph for the specified character size.
			GlyphTable& glyphs = m_pages[characterSize].glyphs;

			// Try to find the glyph in the cache.
			auto iter = glyphs.find(codePoint);
			if (iter != glyphs.end())
			{
				// Return the found glyph.
				return iter->second;
			}
			else
			{
				// No glyph was found in the cache. We better load it!
				Glyph glyph = LoadGlyph(codePoint, characterSize);
				glyphs[codePoint] = glyph;
				return glyphs[codePoint];
			}
		}

		int Font::GetKerning(unsigned int firstCodePoint, unsigned int secondCodePoint, unsigned int characterSize) const
		{
			// Handle the special cases, where one of the two characters is
			// the null character.
			if (firstCodePoint == 0 || secondCodePoint == 0)
			{
				return 0;
			}

			// Try to get the kerning from FreeType.
			FT_Face face = static_cast<FT_Face>(m_face);
			if (face && FT_HAS_KERNING(face) && SetCurrentSize(characterSize))
			{
				// Convert the characters to indices.
				FT_UInt index1 = FT_Get_Char_Index(face, firstCodePoint);
				FT_UInt index2 = FT_Get_Char_Index(face, secondCodePoint);

				// Get the kerning vector.
				FT_Vector kerning;
				FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);

				// Return the x-advance.
				return kerning.x >> 6;
			}
			else
			{
				// Invalid font or no kerning.
				return 0;
			}
		}

		int Font::GetLineSpacing(unsigned int characterSize) const
		{
			FT_Face face = static_cast<FT_Face>(m_face);
			if (face && SetCurrentSize(characterSize))
			{
				return face->size->metrics.height >> 6;
			}
			else
			{
				return 0;
			}
		}

		bool Font::SetCurrentSize(unsigned int characterSize) const
		{
			FT_Face face = static_cast<FT_Face>(m_face);
			FT_UShort currentSize = face->size->metrics.x_ppem;
			if (currentSize == characterSize)
			{
				// Nothing to do if the current character size is equal to the
				// specified desired character size.
				return true;
			}
			else
			{
				// Try to set the current character size.
				return FT_Set_Pixel_Sizes(face, 0, characterSize) == 0;
			}
		}

		IntRectangle Font::FindGlyphRectangle(Font::Page& page, unsigned int width, unsigned int height) const
		{
			// Try to find a row that fits the glyph well.
			Row* row = nullptr;
			float bestRatio = 0.0f;
			for (auto iter = page.rows.begin(); iter != page.rows.end() && !row; ++iter)
			{
				const float ratio = static_cast<float>(height) / iter->height;

				// Ignore rows that are either too small or too tall.
				if (ratio > 1.0f || ratio < 0.7f)
				{
					continue;
				}

				// Ignore rows that don't have enough horizontal space left to
				// accomodate the glyph.
				if (width > page.texture->GetWidth() - iter->width)
				{
					continue;
				}

				// Ignore rows that don't accomodate the glyph better that the
				// previous best row found.
				if (ratio < bestRatio)
				{
					continue;
				}

				// Select the current row as the best so far.
				bestRatio = ratio;
				row = &(*iter);
			}

			// Create a new row in the page if we didn't find an existing row
			// that could accomodate the glyph.
			if (!row)
			{
				// Set the height of the new row to be 10% taller than the
				// glyph - so that we can accomodate a number of glyphs of
				// similar size.
				int rowHeight = height + height / 10;

				// Increase the size of the texture if necessary in order
				// to accomodate the new row.
				while (page.nextRow + rowHeight >= page.texture->GetHeight())
				{
					// Get the current texture dimensions.
					const unsigned int textureWidth = page.texture->GetWidth();
					const unsigned int textureHeight = page.texture->GetHeight();

					// Double the size of the texture.
					const Texture copy = *page.texture;
					page.texture->Create(textureWidth * 2, textureHeight * 2);
					page.texture->Copy(copy, 0, 0);
				}

				// Create the new row.
				page.rows.push_back(Row(page.nextRow, rowHeight));
				page.nextRow += rowHeight;
				row = &(page.rows.back());
			}

			// Determine the glyph's rectangle in the texture.
			IntRectangle glyphRectangle(row->width, row->top, width, height);

			// Update the width of the row.
			row->width += width;

			// Return the glyph's rectangle in the texture.
			return glyphRectangle;
		}

		Glyph Font::LoadGlyph(unsigned int codePoint, unsigned int characterSize) const
		{
			FT_Face face = static_cast<FT_Face>(m_face);

			// Create a new glyph that will be returned.
			Glyph glyph;

			// Set the character size.
			SetCurrentSize(characterSize);

			// Load the glyph corresponding to the specified code point.
			if (FT_Load_Char(face, codePoint, FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT) != 0)
			{
				return glyph;
			}

			// Retrieve the glyph.
			FT_Glyph ftGlyph;
			if (FT_Get_Glyph(face->glyph, &ftGlyph) != 0)
			{
				return glyph;
			}

			// Rasterize the glyph by converting it onto a bitmap.
			FT_Glyph_To_Bitmap(&ftGlyph, FT_RENDER_MODE_NORMAL, 0, 1);
			FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)ftGlyph;
			FT_Bitmap& bitmap = bitmapGlyph->bitmap;

			// Set the glyph's advance.
			// This is the horizontal offset to move to the next glyph.
			glyph.advance = (ftGlyph->advance.x >> 16);

			// Get the bitmap dimensions.
			const int width = bitmap.width;
			const int height = bitmap.rows;

			// Ensure that the bitmap dimensions are valid.
			if (width > 0 && height > 0)
			{
				// Small padding around characters so that smooth filtering
				// doesn't pollute them with pixels from neighbours.
				const unsigned int padding = 1;

				// Get the page of glyphs corresponding to the specified
				// character size.
				Page& page = m_pages[characterSize];

				// Find a good position for the new glyph to be inserted into
				// the texture.
				glyph.textureRectangle = FindGlyphRectangle(page, width + padding * 2, height + padding * 2);

				// Determine the glyph's bounding box.
				glyph.bounds = IntRectangle(
					bitmapGlyph->left - padding,
					-bitmapGlyph->top - padding,
					width + padding * 2,
					height + padding * 2
				);

				// Extract the glyph's pixel data from the bitmap into a
				// temporary pixel buffer.
				std::vector<unsigned char> pixelBuffer;
				pixelBuffer.resize(width * height * 4, 255);
				const unsigned char* pixels = bitmap.buffer;
				if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
				{
					// Pixels are 1 bit monochrome values.
					for (unsigned int y = 0; y < height; ++y)
					{
						for (unsigned int x = 0; x < width; ++x)
						{
							// The color channels remain white, just fill the
							// alpha channels.
							std::size_t index = (y * width + x) * 4 + 3;
							pixelBuffer[index] = ((pixels[x / 8]) & (1 << (7 - (x % 8)))) ? 255 : 0;
						}

						pixels += bitmap.pitch;
					}
				}
				else
				{
					// Pixels are 8 bits gray levels.
					for (unsigned int y = 0; y < height; ++y)
					{
						for (unsigned int x = 0; x < width; ++x)
						{
							// The color channels remain white, just fill the
							// alpha channels.
							std::size_t index = (y * width + x) * 4 + 3;
							pixelBuffer[index] = pixels[x];
						}

						pixels += bitmap.pitch;
					}
				}

				// Write the pixels to the texture.
				const unsigned int x = glyph.textureRectangle.GetLeft() + padding;
				const unsigned int y = glyph.textureRectangle.GetTop() + padding;
				const unsigned int w = glyph.textureRectangle.GetWidth() - padding * 2;
				const unsigned int h = glyph.textureRectangle.GetHeight() - padding * 2;
				Texture glyphTexture;
				glyphTexture.Create(w, h, &pixelBuffer[0]);
				page.texture->Copy(glyphTexture, x, y);
			}

			// Free the FT glyph.
			FT_Done_Glyph(ftGlyph);

			// Force an OpenGL flush so that the font's texture will be updated
			// in all contexts immediately.
			glFlush();

			// Return the loaded glyph.
			return glyph;
		}

		Font::Row::Row(unsigned int rowTop, unsigned int rowHeight)
		: width(0)
		, top(rowTop)
		, height(rowHeight)
		{
			// Nothing to do.
		}

		Font::Page::Page()
		: glyphs()
		, texture(std::make_shared<Texture>())
		, nextRow(0)
		, rows()
		{
			// Initialize the texture to be 128 x 128 pixels, all transparent.
			texture->Create(128, 128);
			texture->SetSmooth(true);
			texture->SetRepeat(false);
		}
	}
}
