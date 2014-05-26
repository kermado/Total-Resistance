#ifndef WINDOWRESIZEEVENT_H
#define WINDOWRESIZEEVENT_H

#include <glm/glm.hpp>

#include <Engine/Event/IEvent.hpp>

namespace Engine
{
	namespace Event
	{
		class WindowResizeEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param width The new width of the window.
			 * @param height The new height of the window.
			 */
			WindowResizeEvent(int width, int height);

			/**
			 * Destructor.
			 */
			~WindowResizeEvent();

			/**
			 * Returns the new width of the window.
			 *
			 * @return Window width (in pixels).
			 */
			int GetWidth() const;

			/**
			 * Returns the new height of the window.
			 *
			 * @return Window height (in pixels).
			 */
			int GetHeight() const;

			/**
			 * Returns the new width dimensions.
			 *
			 * @return Window dimensions (in pixels).
			 */
			glm::vec2 GetDimensions() const;

		private:
			/**
			 * The width of the window.
			 */
			int m_width;

			/**
			 * The height of the window.
			 */
			int m_height;
		};
	}
}

#endif
