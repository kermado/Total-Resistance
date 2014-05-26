#ifndef FIREROCKETSATACQUIREDTARGET_H
#define FIREROCKETSATACQUIREDTARGET_H

#include <memory>

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/Attribute/Transform.hpp>

#include "Event/TargetAcquiredEvent.hpp"
#include "RocketFactory.hpp"

namespace Behaviour
{
	class FireRocketsAtAcquiredTarget : public Engine::Behaviour::IBehaviour
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
		 * @param rocketFactory Shared pointer to the game object factory
		 * to use for instantiating the rockets.
		 * @param delayBetweenLaunchs Minimum delay (in seconds) between
		 * launches.
		 */
		FireRocketsAtAcquiredTarget(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
			std::shared_ptr<Engine::IGameObjectFactory> rocketFactory,
			double delayBetweenLaunches);

		/**
		 * Destructor.
		 */
		~FireRocketsAtAcquiredTarget();

		/**
		 * Launches unguided rockets if sufficient time has elapsed since the
		 * last launch and the angle between the target and the direction that
		 * the Game Object is facing is sufficiently small.
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
		 * Game object factory for constructing rockets.
		 */
		std::shared_ptr<Engine::IGameObjectFactory> m_rocketFactory;

		/**
		 * Minimum delay (in seconds) between launches.
		 */
		double m_delayBetweenLaunches;

		/**
		 * The number of seconds that has elapsed since the last launch.
		 */
		double m_secondsSinceLastLaunch;

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
