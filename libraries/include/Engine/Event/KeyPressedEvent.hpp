#ifndef KEYPRESSEDEVENT_H
#define	KEYPRESSEDEVENT_H

#include <Engine/Event/IEvent.hpp>

namespace Engine
{
	namespace Event
	{
		class KeyPressedEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param key GLFW key code.
			 */
			KeyPressedEvent(int key);

			/**
			 * Destructor.
			 */
			virtual ~KeyPressedEvent();

			/**
			 * Returns the key code that was pressed.
			 *
			 * @return Key that was pressed.
			 */
			int GetKey() const;

		private:
			/**
			 * Code for the key that was pressed.
			 */
			int m_key;
		};
	}
}

#endif
