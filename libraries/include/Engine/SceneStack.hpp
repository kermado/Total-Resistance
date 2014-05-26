#ifndef SCENESTACK_H
#define	SCENESTACK_H

#include <string>
#include <memory>
#include <stack>
#include <map>
#include <functional>
#include <cassert>

#include <Engine/NonCopyable.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/IScene.hpp>
#include <Engine/Event/PushSceneEvent.hpp>
#include <Engine/Event/PopSceneEvent.hpp>

namespace Engine
{
	class SceneStack : private NonCopyable
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param window Shared pointer to the game window.
		 * @param
		 */
		SceneStack(std::shared_ptr<Window> window, std::shared_ptr<ResourceManager> resourceManager);

		/**
		 * Destructor.
		 */
		~SceneStack();

		/**
		 * Update the top scene's state.
		 *
		 * @param deltaTime Time since last update (in seconds).
		 */
		void Update(double deltaTime);

		/**
		 * Draws the user interface for scene at the top of the stack.
		 */
		void DrawUI();

		/**
		 * Renders the scene at the top of the stack.
		 */
		void Render();

		/**
		 * Enqueues an event on the top scene's Event Dispatcher.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void EnqueueEvent(Arguments... args)
		{
			if (!m_stack.empty())
			{
				std::shared_ptr<IScene> scene = m_stack.top();
				scene->EnqueueEvent<EventType>(args...);
			}
		}

		/**
		 * Dispatches an event on the top scene's Event Dispatcher.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void DispatchEvent(Arguments... args)
		{
			if (!m_stack.empty())
			{
				std::shared_ptr<IScene> scene = m_stack.top();
				scene->DispatchEvent<EventType>(args...);
			}
		}

		/**
		 * Registers a scene to be instantiated and pushed to the stack at some
		 * time in the future.
		 *
		 * @param name Unique name for the scene.
		 * @param args... Optional arguments to pass to the scene's constructor.
		 */
		template<typename SceneType, typename... Arguments>
		void RegisterScene(const std::string name, Arguments... args)
		{
			// Verify that the scene name is unique.
			assert (m_factories.find(name) == m_factories.end());

			// Add a lambda function that creates the specified scene
			// to the map of factory functions.
			m_factories[name] = [this, args...] ()
			{
				return std::make_shared<SceneType>(m_window, m_resourceManager,
					m_eventDispatcher, args...);
			};
		}

		/**
		 * Checks whether the scene stack is empty.
		 *
		 * @return True if the stack is empty.
		 */
		bool Empty() const;

		/**
		 * Handles PushSceneEvents.
		 *
		 * @param event Reference to the event.
		 */
		void HandlePushSceneEvent(const Event::PushSceneEvent& event);

		/**
		 * Handles PopSceneEvents.
		 *
		 * @param event Reference to the event.
		 */
		void HandlePopSceneEvent(const Event::PopSceneEvent& event);

		/**
		 * Returns a shared pointer to the scene at the top of the stack.
		 *
		 * @return Shared pointer to the top scene.
		 */
		std::shared_ptr<IScene> Top();

		/**
		 * Clears the scene stack of all scenes.
		 *
		 * @note The scenes will be freed from memory at this point.
		 */
		void Clear();

	protected:
		/**
		 * Processes pending changes.
		 */
		void ProcessPendingChanges();

		/**
		 * Represents a single pending change.
		 */
		struct Change
		{
			/**
			 * Actions that can be scheduled.
			 */
			enum Action
			{
				Push,
				Pop,
				Clear
			};

			/**
			 * Action to be performed.
			 */
			Action action;

			/**
			 * Name of the scene.
			 */
			std::string sceneName;
		};

	private:
		/**
		 * Shared pointer to the game window.
		 */
		std::shared_ptr<Window> m_window;

		/**
		 * Shared pointer to the resource manager.
		 */
		std::shared_ptr<ResourceManager> m_resourceManager;

		/**
		 * Event dispatcher for the scene stack.
		 * This will receive events related to scene stack requests.
		 */
		std::shared_ptr<EventDispatcher> m_eventDispatcher;

		/**
		 * Scene stack data structure.
		 */
		std::stack<std::shared_ptr<IScene>> m_stack;

		/**
		 * Actions pending to perform on the stack.
		 */
		std::vector<Change> m_pendingChanges;

		/**
		 * Scene factories.
		 * Map from [scene name] => [factory function]
		 */
		std::map<std::string, std::function<std::shared_ptr<IScene>()>> m_factories;
	};
}

#endif
