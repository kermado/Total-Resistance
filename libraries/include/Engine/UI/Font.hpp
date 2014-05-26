#ifndef FONT_H
#define FONT_H

#include <string>
#include <map>
#include <memory>

#include <Engine/NonCopyable.hpp>
#include <Engine/Rectangle.hpp>
#include <Engine/Texture.hpp>
#include <Engine/UI/Glyph.hpp>

namespace Engine
{
	namespace UI
	{
		/**
		 * Fonts can be loaded from font files supported by FreeType.
		 *
		 * This class was adapted from SFML (http://www.sfml-dev.org/).
		 */
		class Font : public Engine::NonCopyable
		{
		public:
			/**
			 * Constructor.
			 */
			Font();

			/**
			 * Destructor.
			 */
			~Font();

			/**
			 * Loads the font from the specified file.
			 *
			 * @param filepath Path to the font file.
			 * @return True if the font was loaded successfully.
			 */
			bool LoadFromFile(std::string filepath);

			/**
			 * Returns a shared pointer to the texture containing the loaded
			 * glyphs of the specified character size.
			 *
			 * @param characterSize Reference character size.
			 * @return Shared pointer to the texture containing the glyphs of
			 * the requested character size.
			 */
			std::shared_ptr<Texture> GetTexture(unsigned int characterSize) const;

			/**
			 * Returns the glyph corresponding to the specified unicode code
			 * point and character size.
			 *
			 * @param codePoint Unicode code point for the character.
			 * @param characterSize Reference character size.
			 * @return The glyph corresponding to the specified unicode code
			 * point and character size.
			 */
			const Glyph& GetGlyph(unsigned int codePoint, unsigned int characterSize) const;

			/**
			 * Returns the kerning offset of the two glyphs corresponding to the
			 * specified code points.
			 *
			 * The kerning is an extra negative offset to apply between two
			 * glyphs during rendering in order to make the pair look more
			 * "natural". For example, the pair "AV" has a special kerning to
			 * make the two characters appear closer than other character
			 * pairs.
			 *
			 * @param firstCodePoint Unicode code point for the first character.
			 * @param secondCodePoint Unicode code point for the second
			 * character.
			 * @param characterSize Reference character size.
			 * @return Kerning value for the two characters (in pixels).
			 */
			int GetKerning(unsigned int firstCodePoint, unsigned int secondCodePoint, unsigned int characterSize) const;

			/**
			 * Returns the line spacing.
			 *
			 * Line spacing is the vertical offset to applay between two
			 * consecutive lines of text.
			 *
			 * @param characterSize Reference character size.
			 * @return Line spacing (in pixels).
			 */
			int GetLineSpacing(unsigned int characterSize) const;

		private:
			/**
			 * Table that maps code points to glyphs.
			 */
			typedef std::map<unsigned int, Glyph> GlyphTable;

			/**
			 * Structure representing a row of glyphs.
			 */
			struct Row
			{
				/**
				 * Constructor.
				 *
				 * @param rowTop The y-coordinate for the top of the row.
				 * @param rowHeight The height for the row.
				 */
				Row(unsigned int rowTop, unsigned int rowHeight);

				/**
				 * The width of the row.
				 */
				unsigned int width;

				/**
				 * The y-coordinate of the top of the row.
				 */
				unsigned int top;

				/**
				 * The height of the row.
				 */
				unsigned int height;
			};

			/**
			 * Structure representing a page of glyphs.
			 */
			struct Page : public Engine::NonCopyable
			{
				/**
				 * Constructor.
				 */
				Page();

				/**
				 * Table that maps code points to the correspinding glyphs.
				 */
				GlyphTable glyphs;

				/**
				 * Shared pointer to the texture that contains the pixels of the
				 * rasterized glyphs.
				 */
				std::shared_ptr<Texture> texture;

				/**
				 * The y-coordinate of the next row in the texture.
				 */
				unsigned int nextRow;

				/**
				 * List of all existing rows in the texture.
				 */
				std::vector<Row> rows;
			};

		private:
			/**
			 * Sets the current character size.
			 *
			 * @param characterSize Reference character size.
			 * @return False if there was an error.
			 */
			bool SetCurrentSize(unsigned int characterSize) const;

			/**
			 * Finds and returns a suitable rectangle within the specified page
			 * (texture) for a glyph with the specified dimensions to be
			 * inserted.
			 *
			 * @param page The page of glyphs to search in.
			 * @param width Width of the glyph.
			 * @param height Height of the glyph.
			 */
			IntRectangle FindGlyphRectangle(Page& page, unsigned int width, unsigned int height) const;

			/**
			 * Loads a new glyph.
			 *
			 * @param codePoint Unicode code point for the character.
			 * @param characterSize Reference character size.
			 * @return The glyph corresponding to the specified unicode code
			 * point and character size.
			 */
			Glyph LoadGlyph(unsigned int codePoint, unsigned int characterSize) const;

		private:
			/**
			 * FreeType library interface.
			 */
			void* m_library;

			/**
			 * FreeType font face.
			 */
			void* m_face;

			/**
			 * Maps character size to a page (texture).
			 */
			mutable std::map<unsigned int, Page> m_pages;
		};
	}
}

#endif
