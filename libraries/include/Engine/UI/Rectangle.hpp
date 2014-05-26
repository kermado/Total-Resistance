#ifndef UI_RECTANGLE_H
#define UI_RECTANGLE_H

#include <Engine/UI/IShape.hpp>

namespace Engine
{
	namespace UI
	{
		class Rectangle : public IShape
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param dimensions Dimensions for the rectangle (width, height).
			 */
			Rectangle(const glm::vec2& dimensions = glm::vec2(0.0f, 0.0f));

			/**
			 * Returns the dimensions for the rectangle.
			 *
			 * @return Rectangle dimensions (width, height).
			 */
			glm::vec2 GetDimensions() const;

			/**
			 * Sets the dimensions for the rectangle.
			 *
			 * @param dimensions Dimensions for the rectangle (width, height).
			 */
			void SetDimensions(const glm::vec2& dimensions);

			/**
			 * Returns the width of the rectangle.
			 *
			 * @return Rectangle width.
			 */
			float GetWidth() const;

			/**
			 * Sets the width for the rectangle.
			 *
			 * @param width Width for the rectangle.
			 */
			void SetWidth(float width);

			/**
			 * Returns the height of the rectangle.
			 *
			 * @return Rectangle height.
			 */
			float GetHeight() const;

			/**
			 * Sets the height for the rectangle.
			 *
			 * @param height Height for the rectangle.
			 */
			void SetHeight(float height);

			/**
			 * Returns the number of vertices that compose the rectangle.
			 *
			 * @return Number of vertices that compose the rectangle.
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

		private:
			/**
			 * Rectangle dimensions.
			 */
			glm::vec2 m_dimensions;
		};
	}
}

#endif
