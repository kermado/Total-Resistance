#include <Engine/Event/WindowResizeEvent.hpp>

namespace Engine
{
	namespace Event
	{
		WindowResizeEvent::WindowResizeEvent(int width, int height)
		: m_width(width)
		, m_height(height)
		{
			// Nothing to do.
		}

		WindowResizeEvent::~WindowResizeEvent()
		{
			// Nothing to do.
		}

		int WindowResizeEvent::GetWidth() const
		{
			return m_width;
		}

		int WindowResizeEvent::GetHeight() const
		{
			return m_height;
		}

		glm::vec2 WindowResizeEvent::GetDimensions() const
		{
			return glm::vec2(static_cast<float>(m_width), static_cast<float>(m_height));
		}
	}
}
