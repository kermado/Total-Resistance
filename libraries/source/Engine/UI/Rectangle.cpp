#include <Engine/UI/Rectangle.hpp>

namespace Engine
{
	namespace UI
	{
		Rectangle::Rectangle(const glm::vec2& dimensions)
		: IShape()
		, m_dimensions(dimensions)
		{
			// Nothing to do.
		}

		glm::vec2 Rectangle::GetDimensions() const
		{
			return m_dimensions;
		}

		void Rectangle::SetDimensions(const glm::vec2& dimensions)
		{
			m_dimensions = dimensions;
			ShouldUpdate();
		}

		float Rectangle::GetWidth() const
		{
			return m_dimensions.x;
		}

		void Rectangle::SetWidth(float width)
		{
			m_dimensions.x = width;
			ShouldUpdate();
		}

		float Rectangle::GetHeight() const
		{
			return m_dimensions.y;
		}

		void Rectangle::SetHeight(float height)
		{
			m_dimensions.y = height;
			ShouldUpdate();
		}

		unsigned int Rectangle::GetVertexCount() const
		{
			return 6;
		}

		glm::vec2 Rectangle::GetVertexPosition(unsigned int index) const
		{
			switch (index)
			{
				// Triangle 1
				case 0: return glm::vec2(0.0f, 0.0f);
				case 1: return glm::vec2(m_dimensions.x, 0.0f);
				case 2: return glm::vec2(m_dimensions.x, m_dimensions.y);

				// Triangle 2
				case 3: return glm::vec2(0.0f, 0.0f);
				case 4: return glm::vec2(0.0f, m_dimensions.y);
				case 5: return glm::vec2(m_dimensions.x, m_dimensions.y);

				// Out of range.
				default: assert(0);
			}
		}
	}
}
