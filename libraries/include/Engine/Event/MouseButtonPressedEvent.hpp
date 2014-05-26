#ifndef MOUSEBUTTONPRESSEDEVENT_H
#define	MOUSEBUTTONPRESSEDEVENT_H

#include <Engine/Event/IEvent.hpp>

#include <glm/glm.hpp>

namespace Engine
{
	namespace Event
	{
		class MouseButtonPressedEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param button Code for the button that was pressed.
			 * @param coordinates Viewport coordinates of the cursor at the time
			 * that the mouse button was pressed
			 */
			MouseButtonPressedEvent(int button, const glm::vec2& coordinates);

			/**
			 * Destructor.
			 */
			virtual ~MouseButtonPressedEvent();

			/**
			 * Returns the button that was pressed.
			 *
			 * @return Button pressed.
			 */
			int GetButton() const;

			/**
			 * Returns the mouse coordinates as a 2D vector.
			 *
			 * @return 2D vector of mouse coordinates (x, y).
			 */
			const glm::vec2& GetCoordinates() const;

		private:
			/**
			 * The button that was pressed.
			 */
			int m_button;

			/**
			 * Mouse coordinates (x, y).
			 */
			glm::vec2 m_coordinates;
		};
	}
}

#endif
