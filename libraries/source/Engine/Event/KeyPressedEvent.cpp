#include <Engine/Event/KeyPressedEvent.hpp>

namespace Engine
{
	namespace Event
	{
		KeyPressedEvent::KeyPressedEvent(int key)
		: IEvent()
		, m_key(key)
		{
			// Nothing to do.
		}

		KeyPressedEvent::~KeyPressedEvent()
		{
			// Nothing to do.
		}

		int KeyPressedEvent::GetKey() const
		{
			return m_key;
		}
	}
}
