#include <Engine/Event/MouseButtonPressedEvent.hpp>

namespace Engine
{
	namespace Event
	{
		MouseButtonPressedEvent::MouseButtonPressedEvent(int button, const glm::vec2& coordinates)
		: IEvent()
		, m_button(button)
		, m_coordinates(coordinates)
		{
			// Nothing to do.
		}

		MouseButtonPressedEvent::~MouseButtonPressedEvent()
		{
			// Nothing to do.
		}

		int MouseButtonPressedEvent::GetButton() const
		{
			return m_button;
		}

		const glm::vec2& MouseButtonPressedEvent::GetCoordinates() const
		{
			return m_coordinates;
		}
	}
}
