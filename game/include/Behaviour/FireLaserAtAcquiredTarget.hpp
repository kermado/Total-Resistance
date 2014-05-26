#ifndef FIRELASERATACQUIREDTARGET_H
#define FIRELASERATACQUIREDTARGET_H

#include <memory>

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/Attribute/Transform.hpp>

#include "Event/TargetAcquiredEvent.hpp"
#include "RocketFactory.hpp"

namespace Behaviour
{
	class FireLaserAtAcquiredTarget : public Engine::Behaviour::IBehaviour
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
		 */
		FireLaserAtAcquiredTarget(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Transform> transformAttribute);

		/**
		 * Destructor.
		 */
		~FireLaserAtAcquiredTarget();

		/**
		 * Starts the laser if the angle between the target and the direction
		 * that the Game Object is facing is sufficiently small. Stops firing
		 * the laser otherwise.
		 */
		virtual void Update(double deltaTime);

	private:
		/**
		 * Starts firing the laser.
		 */
		void StartLaser();

		/**
		 * Stops firing the laser.
		 */
		void StopLaser();

	private:
		/**
		 * Shared pointer to the game object's transform attribute.
		 */
		std::shared_ptr<Engine::Attribute::Transform> m_transformAttribute;

		/**
		 * Shared pointer to the laser beam Game Object.
		 */
		std::shared_ptr<Engine::GameObject> m_laser;

		/**
		 * Subscription identifier for the TargetAcquiredEvent subscription.
		 */
		Engine::EventDispatcher::SubscriptionID m_targetAcquiredSubscription;

		/**
		 * The target that was acquired.
		 */
		std::shared_ptr<Engine::GameObject> m_target;
	};
}

#endif
