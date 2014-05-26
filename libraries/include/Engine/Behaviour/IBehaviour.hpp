#ifndef IBEHAVIOUR_H
#define	IBEHAVIOUR_H

#include <memory>

#include <Engine/NonCopyable.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>

namespace Engine
{
	class GameObject;

	namespace Behaviour
	{
		class IBehaviour : private NonCopyable
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
			 * behaviour is attached to.
			 */
			IBehaviour(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject);

			/**
			 * Destructor.
			 */
			virtual ~IBehaviour();

			/**
			 * Implement this method to perform actions at regular intervals.
			 *
			 * @param deltaTime Time since last call (in seconds).
			 */
			virtual void Update(double deltaTime) = 0;

		protected:
			/**
			 * Returns a shared pointer to the main game window.
			 *
			 * @return Shared pointer to the window.
			 */
			std::shared_ptr<Window> GetWindow() const;

			/**
			 * Returns a shared pointer to the resource manager.
			 *
			 * @return Shared pointer to the resource manager.
			 */
			std::shared_ptr<ResourceManager> GetResourceManager() const;

			/**
			 * Returns a shared pointer to the scene's event dispatcher.
			 *
			 * @return Shared pointer to the scene's event dispatcher.
			 */
			std::shared_ptr<EventDispatcher> GetSceneEventDispatcher() const;

			 /**
			  * Returns a shared pointer to the game object's event dispatcher.
			  *
			  * @return Shared pointer to the game object's event dispatcher.
			  */
			std::shared_ptr<EventDispatcher> GetGameObjectEventDispatcher() const;

			/**
			 * Returns a weak pointer to the game object that the behaviour is
			 * attached to.
			 *
			 * @return Weak pointer to the game object.
			 */
			std::weak_ptr<GameObject> GetGameObject() const;

		private:
			/**
			 * Shared pointer to the window.
			 */
			std::shared_ptr<Window> m_window;

			/**
			 * Shared pointer to the resource manager.
			 */
			std::shared_ptr<ResourceManager> m_resourceManager;

			/**
			 * Shared pointer to the event dispatcher for the scene that the
			 * Game Object belongs to.
			 */
			std::shared_ptr<EventDispatcher> m_sceneEventDispatcher;

			/**
			 * Shared pointer to the event dispatcher for the Game Object
			 */
			std::shared_ptr<EventDispatcher> m_gameObjectEventDispatcher;

			/**
			 * Weak pointer to the game object that the behaviour is attached
			 * to.
			 */
			std::weak_ptr<GameObject> m_gameObject;
		};
	}
}

#endif
