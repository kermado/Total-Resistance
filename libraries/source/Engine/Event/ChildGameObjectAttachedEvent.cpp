#include <Engine/Event/ChildGameObjectAttachedEvent.hpp>

namespace Engine
{
	namespace Event
	{
		ChildGameObjectAttachedEvent::ChildGameObjectAttachedEvent(std::shared_ptr<GameObject> child)
		: IEvent()
		, m_child(child)
		{
			// Nothing to do.
		}

		ChildGameObjectAttachedEvent::~ChildGameObjectAttachedEvent()
		{
			// Nothing to do.
		}

		std::shared_ptr<GameObject> ChildGameObjectAttachedEvent::GetChild() const
		{
			return m_child;
		}
	}
}
