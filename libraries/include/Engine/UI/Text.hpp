#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <Engine/UI/IShape.hpp>
#include <Engine/UI/Font.hpp>

namespace Engine
{
	namespace UI
	{
		class Text : public IShape
		{
		public:
			/**
			 * Constructor.
			 */
			Text();

			/**
			 * Destructor.
			 */
			virtual ~Text();

			/**
			 * Returns the dimensions for the text.
			 *
			 * @return Text dimensions (width, height).
			 */
			glm::vec2 GetDimensions() const;

			/**
			 * Returns the width of the text.
			 *
			 * @return Text width.
			 */
			float GetWidth() const;

			/**
			 * Returns the height of the text.
			 *
			 * @return Text height.
			 */
			float GetHeight() const;

			/**
			 * Sets the specified font to be used for rendering the text.
			 *
			 * @param font Font to use for rendering the text.
			 */
			void SetFont(const Font& font);

			/**
			 * Returns the current text string.
			 *
			 * @return Current text string.
			 */
			std::string GetString() const;

			/**
			 * Sets the string to be displayed.
			 *
			 * @param string String to be displayed.
			 */
			void SetString(std::string string);

			/**
			 * Sets the character size for the text.
			 *
			 * @param characterSize Character size (in pixels).
			 */
			void SetCharacterSize(unsigned int characterSize);

			/**
			 * Returns the number of vertices that compose the text.
			 *
			 * @return Number of vertices that compose the text.
			 */
			virtual unsigned int GetVertexCount() const;

		protected:
			/**
			 * Returns the position of the vertex specified by the supplied
			 * index.
			 *
			 * @param index Index for the vertex position to retrieve. This must
			 * be in the range [0, GetVertexCount() - 1].
			 * @return The position of the index-th vertex.
			 */
			virtual glm::vec2 GetVertexPosition(unsigned int index) const;

			/**
			 * Returns the UV texture coordinates for the vertex specified by
			 * the provided index.
			 *
			 * @param index Vertex index for the UV coordinate to return.
			 * @return UV texture coordinate for the specified vertex index.
			 */
			virtual glm::vec2 GetTextureCoordinate(unsigned int index) const;

		private:
			/**
			 * Updates the text's geometry.
			 */
			void UpdateGeometry();

		private:
			/**
			 * The font to use for rendering the text.
			 */
			const Font* m_font;

			/**
			 * The text string to be displayed.
			 */
			std::string m_string;

			/**
			 * The character size (in pixels).
			 */
			unsigned int m_characterSize;

			/**
			 * The vertex positions.
			 */
			std::vector<glm::vec2> m_vertexPositions;

			/**
			 * The texture coordinates.
			 */
			std::vector<glm::vec2> m_textureCoordinates;

			/**
			 * Bounding rectangle for the text.
			 */
			IntRectangle m_boundingRectangle;
		};
	}
}

#endif
