#include "Event/EnemyDestroyedEvent.hpp"

namespace Event
{
	EnemyDestroyedEvent::EnemyDestroyedEvent(std::shared_ptr<Engine::GameObject> gameObject)
	: IEvent()
	, m_gameObject(gameObject)
	{
		// Nothing to do.
	}

	EnemyDestroyedEvent::~EnemyDestroyedEvent()
	{
		// Nothing to do.
	}

	std::shared_ptr<Engine::GameObject> EnemyDestroyedEvent::GetGameObject() const
	{
		return m_gameObject;
	}
}
