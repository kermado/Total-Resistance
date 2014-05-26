#ifndef FIREMISSILEATACQUIREDTARGET_H
#define FIREMISSILEATACQUIREDTARGET_H

#include <memory>

#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/Model.hpp>

#include "Event/TargetAcquiredEvent.hpp"

#include "Attribute/Tags.hpp"

#include "PlayingSurface.hpp"

namespace Behaviour
{
	class FireMissileAtAcquiredTarget : public Engine::Behaviour::IBehaviour
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
		 * @param modelAttribute Shared pointer to the game object's model
		 * attribute.
		 * @param tagsAttribute Shared pointer to the game object's tags
		 * attribute.
		 * @param missileFactory Shared pointer to the game object factory
		 * to use for instantiating the missiles.
		 * @param delayBetweenLaunchs Minimum delay (in seconds) between
		 * launches.
		 */
		FireMissileAtAcquiredTarget(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
			std::shared_ptr<Engine::Attribute::Model> modelAttribute,
			std::shared_ptr<Attribute::Tags> tagsAttribute,
			std::shared_ptr<Engine::IGameObjectFactory> missileFactory,
			double delayBetweenLaunches);

		/**
		 * Destructor.
		 */
		~FireMissileAtAcquiredTarget();

		/**
		 * Launches unguided rockets if sufficient time has elapsed since the
		 * last launch and the angle between the target and the direction that
		 * the Game Object is facing is sufficiently small.
		 */
		virtual void Update(double deltaTime);

	private:
		/**
		 * States for the missile silo.
		 */
		enum State
		{
			ConstructingMissile,
			OpeningDoor,
			ClosingDoor
		};

	private:
		/**
		 * Shared pointer to the game object's transform attribute.
		 */
		std::shared_ptr<Engine::Attribute::Transform> m_transformAttribute;

		/**
		 * Shared pointer to the game object's model attribute.
		 */
		std::shared_ptr<Engine::Attribute::Model> m_modelAttribute;

		/**
		 * Shared pointer to the game object's tags attribute.
		 */
		std::shared_ptr<Attribute::Tags> m_tagsAttribute;

		/**
		 * Game object factory for constructing missiles.
		 */
		std::shared_ptr<Engine::IGameObjectFactory> m_missileFactory;

		/**
		 * Minimum delay (in seconds) between launches.
		 */
		double m_delayBetweenLaunches;

		/**
		 * The number of seconds that has elapsed since the last launch.
		 */
		double m_secondsSinceLastLaunch;

		/**
		 * The current state.
		 */
		State m_state;

		/**
		 * Shared pointer to the missile that has been constructed and is
		 * ready for launching.
		 */
		std::shared_ptr<Engine::GameObject> m_missile;

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
