#ifndef ENEMYDESTROYEDEVENT_H
#define ENEMYDESTROYEDEVENT_H

#include <memory>

#include <Engine/Event/IEvent.hpp>
#include <Engine/GameObject.hpp>

namespace Event
{
	class EnemyDestroyedEvent : Engine::Event::IEvent
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param gameObject Shared pointer to the destroyed enemy game object.
		 */
		EnemyDestroyedEvent(std::shared_ptr<Engine::GameObject> gameObject);

		/**
		 * Destructor.
		 */
		~EnemyDestroyedEvent();

		/**
		 * Returns a shared pointer to the destroyed enemy game object.
		 *
		 * @return Shared pointer to the destroyed enemy game object.
		 */
		std::shared_ptr<Engine::GameObject> GetGameObject() const;

	private:
		/**
		 * Shared pointer to the enemy game object that was destroyed.
		 */
		std::shared_ptr<Engine::GameObject> m_gameObject;
	};
}

#endif
