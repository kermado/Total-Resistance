#ifndef MISSILECONTROLLER_H
#define MISSILECONTROLLER_H

#include <memory>

#include <glm/glm.hpp>

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/EventDispatcher.hpp>
#include <Engine/BezierCurve.hpp>

#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/Attribute/Transform.hpp>

namespace Behaviour
{
	class MissileController : public Engine::Behaviour::IBehaviour
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
		 * @param speed Movement speed (world coordinates per second).
		 * @param maxAltitude Maximum altitude (world coordinates).
		 */
		MissileController(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
			float speed,
			float maxAltitude);

		/**
		 * Destructor.
		 */
		~MissileController();

		/**
		 * Moves the game object's transform attribute to follow a pre-computed
		 * path.
		 */
		virtual void Update(double deltaTime);

	private:
		/**
		 * Shared pointer to the game object's transform attribute.
		 */
		std::shared_ptr<Engine::Attribute::Transform> m_transformAttribute;

		/**
		 * The movement speed (in world distance per second).
		 */
		float m_speed;

		/**
		 * The maximum altitude (in world coordinates).
		 */
		float m_maxAltitude;

		/**
		 * The flight path to follow.
		 */
		std::unique_ptr<Engine::BezierCurve<3, glm::vec3>> m_flightPath;

		/**
		 * The current time.
		 */
		double m_time;

		/**
		 * Game Object factory to use for creating exhaust clouds.
		 */
		std::shared_ptr<Engine::IGameObjectFactory> m_exhaustFactory;

		/**
		 * Subscription identifier for the UpdateDestinationEvent subscription.
		 */
		Engine::EventDispatcher::SubscriptionID m_updateDestinationSubscription;
	};
}

#endif
