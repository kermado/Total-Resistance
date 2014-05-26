#ifndef ACQUIRENEARESTTARGET_H
#define ACQUIRENEARESTTARGET_H

#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/EventDispatcher.hpp>
#include <Engine/Attribute/Transform.hpp>
#include <Engine/Event/CollisionEvent.hpp>

#include "Attribute/Tags.hpp"

namespace Behaviour
{
	class AcquireNearestTarget : public Engine::Behaviour::IBehaviour
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param window Shared pointer to the window.
		 * @param resourceManager Shared pointer to the resource manager.
		 * @param sceneEventDispatcher Shared pointer to the scene's event
		 * dispatcher.
		 * @param gameObjectEventDispatcher Shared pointer to the game
		 * object's event dispatcher.
		 * @param gameObject Weak pointer to the Game Object that the
		 * behaviour is attached to.
		 * @param transformAttribute Shared pointer to the game object's
		 * transform attribute.
		 * @param tagsAttribute Shared pointer to the game object's tags
		 * attribute.
		 * @param tag Tag that determines that Game Objects to be targeted.
		 */
		AcquireNearestTarget(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
			std::shared_ptr<Attribute::Tags> tagsAttribute,
			std::string tag);

		/**
		 * Destructor.
		 */
		~AcquireNearestTarget();

		/**
		 * Finds the nearest Game Object having the specified tag and raises an
		 * event to notify other behaviours.
		 *
		 * @param deltaTime Time since last call (in seconds).
		 */
		virtual void Update(double deltaTime);

		/**
		 * Handles CollisionEvents.
		 *
		 * @param event Reference to the event.
		 */
		void HandleCollisionEvent(const Engine::Event::CollisionEvent& event);

	private:
		/**
		 * Shared pointer to the game object's transform attribute.
		 */
		std::shared_ptr<Engine::Attribute::Transform> m_transformAttribute;

		/**
		 * Shared pointer to the game object's tags attribute.
		 */
		std::shared_ptr<Attribute::Tags> m_tagsAttribute;

		/**
		 * The tag to target.
		 */
		std::string m_tag;

		/**
		 * Subscription identifier for the CollisionEvent subscription.
		 */
		Engine::EventDispatcher::SubscriptionID m_collisionSubscription;

		/**
		 * Game Objects within range.
		 */
		std::vector<std::shared_ptr<Engine::GameObject>> m_targetsWithinRange;
	};
}

#endif
