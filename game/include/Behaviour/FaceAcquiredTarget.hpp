#ifndef FACEACQUIREDTARGET_H
#define FACEACQUIREDTARGET_H

#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/Attribute/Transform.hpp>
#include <Engine/GameObject.hpp>

#include "Event/TargetAcquiredEvent.hpp"

namespace Behaviour
{
	class FaceAcquiredTarget : public Engine::Behaviour::IBehaviour
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
		 * @param rotationSpeed Speed at which to rotate (radians per second).
		 */
		FaceAcquiredTarget(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
			float rotationSpeed);

		/**
		 * Destructor.
		 */
		~FaceAcquiredTarget();

		/**
		 * Rotates the Game Object to face the target that has been acquired.
		 *
		 * @param deltaTime Time since last call (in seconds).
		 */
		virtual void Update(double deltaTime);

		/**
		 * Handles TargetAcquiredEvents.
		 *
		 * @param event Reference to the event.
		 */
		void HandleTargetAcquiredEvent(const Event::TargetAcquiredEvent& event);

	private:
		/**
		 * Shared pointer to the game object's transform attribute.
		 */
		std::shared_ptr<Engine::Attribute::Transform> m_transformAttribute;

		/**
		 * Subscription identifier for the TargetAcquiredEvent subscription.
		 */
		Engine::EventDispatcher::SubscriptionID m_targetAcquiredSubscription;

		/**
		 * The target that was acquired.
		 */
		std::shared_ptr<Engine::GameObject> m_target;

		/**
		 * The rotation speed in radians per second.
		 */
		float m_rotationSpeed;
	};
}

#endif
