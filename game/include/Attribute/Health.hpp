#ifndef HEALTH_H
#define HEALTH_H

#include <Engine/Attribute/IAttribute.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>

#include "Event/InflictDamageEvent.hpp"

namespace Attribute
{
	class Health : public Engine::Attribute::IAttribute
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param window Shared pointer to the window.
		 * @param resourceManager Shared pointer to the resource manager.
		 * @param sceneEventDispatcher Shared pointer to the scene's Event
		 * Dispatcher.
		 * @param gameObjectEventDispatcher Shared pointer to the Game
		 * Object's Event Dispatcher.
		 * @param gameObject Weak pointer to the Game Object that the
		 * attribute is attached to.
		 * @param initialHealth Initial amount of health.
		 */
		Health(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			float initialHealth);

		/**
		 * Destructor.
		 */
		~Health();

		/**
		 * Returns the current amount of health.
		 *
		 * @return Amount of health.
		 */
		float GetHealth() const;

		/**
		 * Handles InflictDamageEvents.
		 * A DestroyGameObjectEvent will be raised in the event that is no
		 * health left after inflicting damage.
		 *
		 * @param event Reference to the event.
		 */
		void HandleInflictDamageEvent(const Event::InflictDamageEvent& event);

	private:
		/**
		 * The current health.
		 */
		float m_health;

		/**
		 * Dead flag - used to ensure that we don't publish multiple
		 * EnemyDestroyedEvents if there are multiple requests to inflict damage
		 * in a single state update.
		 */
		bool m_dead;

		/**
		 * Subscription identifier for the InflictDamageEvent subscription
		 */
		Engine::EventDispatcher::SubscriptionID m_inflictDamageSubscription;
	};
}

#endif
