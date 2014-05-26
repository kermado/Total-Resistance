#ifndef APPLICATION_H
#define	APPLICATION_H

#include <string>
#include <memory>

#include <Engine/NonCopyable.hpp>
#include <Engine/Window.hpp>
#include <Engine/SceneStack.hpp>
#include <Engine/ResourceManager.hpp>

namespace Engine
{
	class Application : private NonCopyable
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param width Width for the window (in pixels).
		 * @param height Height for the window (in pixels).
		 * @param title Window title.
		 */
		Application(unsigned int width, unsigned int height, std::string title);

		/**
		 * Destructor.
		 */
		virtual ~Application();

		/**
		 * Registers a scene to be instantiated and pushed to the scene stack at
		 * some time in the future.
		 *
		 * @param name Unique name for the scene.
		 * @param args... Optional arguments to pass to the scene's constructor.
		 */
		template<typename SceneType, typename... Arguments>
		void RegisterScene(std::string name, Arguments... args)
		{
			m_sceneStack->RegisterScene<SceneType>(name, args...);
		}

		/**
		 * Pushes the registered scene to the scene stack that matches the
		 * specified name.
		 *
		 * @param name Unique name for the scene.
		 */
		void PushScene(std::string name);

		/**
		 * Starts the main loop.
		 */
		void Run();

	protected:
		/**
		 * Renders the game state.
		 */
		void Render();

	private:
		/**
		 * Is the main loop running?
		 */
		bool m_running;

		/**
		 * Window for the main thread.
		 */
		std::shared_ptr<Window> m_mainWindow;

		/**
		 * Scene stack.
		 */
		std::unique_ptr<SceneStack> m_sceneStack;

		/**
		 * Resource manager.
		 */
		std::shared_ptr<ResourceManager> m_resourceManager;
	};
}

#endif
