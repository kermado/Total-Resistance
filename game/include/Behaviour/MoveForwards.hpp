#ifndef MOVEFORWARDS_H
#define MOVEFORWARDS_H

#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/Attribute/Transform.hpp>

namespace Behaviour
{
	class MoveForwards : public Engine::Behaviour::IBehaviour
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
		 */
		MoveForwards(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
			float speed);

		/**
		 * Destructor.
		 */
		~MoveForwards();

		/**
		 * Moves the game object's transform attribute in the forwards
		 * direction.
		 */
		virtual void Update(double deltaTime);

	private:
		/**
		 * Shared pointer to the game object's transform attribute.
		 */
		std::shared_ptr<Engine::Attribute::Transform> m_transformAttribute;

		/**
		 * The movement speed in distance (world coordinates) per second.
		 */
		float m_speed;
	};
}

#endif
