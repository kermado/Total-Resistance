#include <Engine/UI/Text.hpp>

#include <iostream>

namespace Engine
{
	namespace UI
	{
		Text::Text()
		: IShape()
		, m_font(nullptr)
		, m_string("")
		, m_characterSize(30)
		, m_vertexPositions()
		, m_textureCoordinates()
		, m_boundingRectangle()
		{
			// Nothing to do.
		}

		Text::~Text()
		{
			// Nothing to do.
		}

		glm::vec2 Text::GetDimensions() const
		{
			return glm::vec2(
				static_cast<float>(m_boundingRectangle.GetWidth()),
				static_cast<float>(m_boundingRectangle.GetHeight())
			);
		}

		float Text::GetWidth() const
		{
			return static_cast<float>(m_boundingRectangle.GetWidth());
		}

		float Text::GetHeight() const
		{
			return static_cast<float>(m_boundingRectangle.GetHeight());
		}

		void Text::SetFont(const Font& font)
		{
			if (&font != m_font)
			{
				m_font = &font;
				UpdateGeometry();
				SetTexture(m_font->GetTexture(m_characterSize));
				ShouldUpdate();
			}
		}

		std::string Text::GetString() const
		{
			return m_string;
		}

		void Text::SetString(std::string string)
		{
			if (string != m_string)
			{
				m_string = string;
				UpdateGeometry();
				ShouldUpdate();
			}
		}

		void Text::SetCharacterSize(unsigned int characterSize)
		{
			if (characterSize != m_characterSize)
			{
				m_characterSize = characterSize;
				UpdateGeometry();
				SetTexture(m_font->GetTexture(m_characterSize));
				ShouldUpdate();
			}
		}

		unsigned int Text::GetVertexCount() const
		{
			// Determine the number of glyphs.
			unsigned int glyphCount = 0;
			for (std::size_t i = 0; i < m_string.length(); ++i)
			{
				const char currentCharacter = m_string[i];
				if (currentCharacter != ' ' && currentCharacter != '\t' &&
					currentCharacter != '\n' && currentCharacter != '\v')
				{
					glyphCount += 1;
				}
			}

			// Each glyph will be composed by 2 triangles.
			return glyphCount * 6;
		}

		glm::vec2 Text::GetVertexPosition(unsigned int index) const
		{
			// Return the requested vertex position.
			assert(index < GetVertexCount());
			return m_vertexPositions[index];
		}

		glm::vec2 Text::GetTextureCoordinate(unsigned int index) const
		{
			// Return the requested texture coordinate.
			assert(index < m_textureCoordinates.size());
			const glm::vec2 textureDimensions = m_font->GetTexture(m_characterSize)->GetDimensions();
			return m_textureCoordinates[index] / textureDimensions;
		}

		void Text::UpdateGeometry()
		{
			// Clear old geometry data.
			m_vertexPositions.clear();
			m_textureCoordinates.clear();

			// Reserve enough space for the buffers.
			m_vertexPositions.reserve(GetVertexCount());
			m_textureCoordinates.reserve(GetVertexCount());

			// We can't proceed if we don't have a valid font or string.
			if (!m_font || m_string.empty())
			{
				return;
			}

			// Get the character spacing.
			const float hSpace = static_cast<float>(m_font->GetGlyph(' ', m_characterSize).advance);
			const float vSpace = static_cast<float>(m_font->GetLineSpacing(m_characterSize));

			// Holds the text bounding rectangle geometry.
			float minX = 0.0f;
			float minY = 0.0f;
			float maxX = 0.0f;
			float maxY = 0.0f;

			// Holds the coordinate offsets for the glyph rectangle.
			float x = 0.0f;
			float y = static_cast<float>(m_characterSize);

			// Holds the previous character processed.
			// Note that the previous character is initially the null character.
			unsigned int previousCharacter = 0;

			// For each character...
			for (std::size_t i = 0; i < m_string.length(); ++i)
			{
				// The current character that we are processing.
				unsigned int currentCharacter = m_string[i];

				// Apply kerning offset.
				x += static_cast<float>(m_font->GetKerning(previousCharacter, currentCharacter, m_characterSize));
				previousCharacter = currentCharacter;

				// Handle special characters.
				if (currentCharacter == ' ' || currentCharacter == '\t' ||
					currentCharacter == '\n' || currentCharacter == '\v')
				{
					// Adjust the vertex positions appropriately.
					switch (currentCharacter)
					{
						case ' ' : x += hSpace; break;
						case '\t' : x += hSpace * 4.0f; break;
						case '\n' : y += vSpace; x = 0.0f; break;
						case '\v' : y += vSpace * 4.0f; break;
					}

					// Update the bounding rectangle geometry.
					maxX = std::max(maxX, x);
					maxY = std::max(maxY, y);

					// There is no need to create vertices for white space.
					continue;
				}

				// Get the glyph for the current character from the font.
				const Glyph& glyph = m_font->GetGlyph(currentCharacter, m_characterSize);

				// Get the positions of the 4 corners of the glyph's bounding
				// rectangle, taking into account the previously calculated
				// coordinate offsets.
				const float left = x + static_cast<float>(glyph.bounds.GetLeft());
				const float top = y + static_cast<float>(glyph.bounds.GetTop());
				const float right = left + static_cast<float>(glyph.bounds.GetWidth());
				const float bottom = top + static_cast<float>(glyph.bounds.GetHeight());

				// Update the bounding rectangle geometry.
				minX = std::min(minX, left);
				maxX = std::max(maxX, right);
				minY = std::min(minY, top);
				maxY = std::max(maxY, bottom);

				// Store the bounding rectangle geometry.
				m_boundingRectangle.SetLeft(minX);
				m_boundingRectangle.SetTop(minY);
				m_boundingRectangle.SetWidth(maxX - minX);
				m_boundingRectangle.SetHeight(maxY - minY);

				// Store the vertex positions in the buffer.
				m_vertexPositions.push_back(glm::vec2(left, top));
				m_vertexPositions.push_back(glm::vec2(right, top));
				m_vertexPositions.push_back(glm::vec2(right, bottom));
				m_vertexPositions.push_back(glm::vec2(left, top));
				m_vertexPositions.push_back(glm::vec2(left, bottom));
				m_vertexPositions.push_back(glm::vec2(right, bottom));

				// Get the texture coordinates for the glyph.
				const float texLeft = static_cast<float>(glyph.textureRectangle.GetLeft());
				const float texTop = static_cast<float>(glyph.textureRectangle.GetTop());
				const float texRight = texLeft + static_cast<float>(glyph.textureRectangle.GetWidth());
				const float texBottom = texTop + static_cast<float>(glyph.textureRectangle.GetHeight());

				// Store the texture coordinates in the buffer.
				m_textureCoordinates.push_back(glm::vec2(texLeft, texTop));
				m_textureCoordinates.push_back(glm::vec2(texRight, texTop));
				m_textureCoordinates.push_back(glm::vec2(texRight, texBottom));
				m_textureCoordinates.push_back(glm::vec2(texLeft, texTop));
				m_textureCoordinates.push_back(glm::vec2(texLeft, texBottom));
				m_textureCoordinates.push_back(glm::vec2(texRight, texBottom));

				// Advance to the next character.
				x += static_cast<float>(glyph.advance);
			}
		}
	}
}
