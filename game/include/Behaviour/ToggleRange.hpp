#ifndef TOGGLERANGE_H
#define TOGGLERANGE_H

#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/Attribute/Model.hpp>

namespace Behaviour
{
	class ToggleRange : public Engine::Behaviour::IBehaviour
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
		 * @param modelAttribute Shared pointer to the game object's
		 * model attribute.
		 */
		ToggleRange(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Model> modelAttribute);

		/**
		 * Destructor.
		 */
		~ToggleRange();

		/**
		 * Does nothing.
		 */
		virtual void Update(double deltaTime);

	private:
		/**
		 * Shared pointer to the game object's model attribute.
		 */
		std::shared_ptr<Engine::Attribute::Model> m_modelAttribute;

		/**
		 * Subscription identifier for the ToggleRangeEvent subscription.
		 */
		Engine::EventDispatcher::SubscriptionID m_toggleRangeSubscription;
	};
}

#endif
