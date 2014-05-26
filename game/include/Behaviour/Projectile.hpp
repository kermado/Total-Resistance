#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>

#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/Attribute/Transform.hpp>
#include <Engine/Event/CollisionEvent.hpp>

#include "PlayingSurface.hpp"

namespace Behaviour
{
	class Projectile : public Engine::Behaviour::IBehaviour
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
		 * @param playingSurface The playing surface over which the projectile
		 * moves.
		 * @param tag The tag that the projectile can hit.
		 * @param damage Amount of damage to be inflicted.
		 */
		Projectile(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
			const PlayingSurface& playingSurface,
			std::string tag,
			float damage);

		/**
		 * Destructor.
		 */
		~Projectile();

		/**
		 * Requests to destroy the Game Object if it is outside the bounds of
		 * the playing surface.
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
		 * The playing surface over which the projectile moves.
		 */
		PlayingSurface m_playingSurface;

		/**
		 * The tag that the projectile can hit.
		 */
		std::string m_tag;

		/**
		 * The amount of damage that should be inflicted.
		 */
		float m_damage;

		/**
		 * Subscription identifier for the CollisionEvent subscription.
		 */
		Engine::EventDispatcher::SubscriptionID m_collisionSubscription;
	};
}

#endif
