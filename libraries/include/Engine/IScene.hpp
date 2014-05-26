#ifndef ISCENE_H
#define	ISCENE_H

#include <memory>

#include <Engine/NonCopyable.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/UI/Renderer.hpp>
#include <Engine/UI/IShape.hpp>

namespace Engine
{
	class IScene : private NonCopyable
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param window Shared pointer to the window.
		 * @param resourceManager Shared pointer to the resource manager.
		 * @param sceneStackEventDispatcher Shared pointer to the scene stack's
		 * event dispatcher.
		 */
		IScene(std::shared_ptr<Window> window, std::shared_ptr<ResourceManager> resourceManager,
			std::shared_ptr<EventDispatcher> sceneStackEventDispatcher);

		/**
		 * Destructor.
		 */
		virtual ~IScene();

		/**
		 * Enqueues an event to the scene's Event Dispatcher.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void EnqueueEvent(Arguments... args)
		{
			m_eventDispatcher->Enqueue<EventType>(args...);
		}

		/**
		 * Dispatches an event to the scene's Event Dispatcher.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void DispatchEvent(Arguments... args)
		{
			m_eventDispatcher->Dispatch<EventType>(args...);
		}

		/**
		 * Updates the scene.
		 *
		 * This is a template method that calls the OnUpdate() method on derived
		 * classes.
		 *
		 * @param deltaTime Time since last call (in seconds).
		 */
		virtual void Update(const double deltaTime);

		/**
		 * Renders the scene.
		 */
		virtual void Render();

		/**
		 * Implement this method to perform actions after the scene has been created.
		 *
		 * Called immediately after the scene has been created.
		 *
		 * This method should be used for setting up the scene by creating
		 * Game Objects and UI elements.
		 */
		virtual void OnCreate() = 0; // TODO: Use events instead.

		/**
		 * Implement this method to perform actions before the scene has been suspended.
		 *
		 * Called immediately before the scene looses focus and is no longer at
		 * the top of the scene stack.
		 */
		virtual void OnSuspend() = 0; // TODO: Use events instead.

		/**
		 * Implement this method to perform actions when the scene has been resumed.
		 *
		 * Called immediately after the scene regains focus and is at the top
		 * of the scene stack.
		 */
		virtual void OnResume() = 0; // TODO: Use events instead.

		/**
		 * Draws the user interface.
		 */
		virtual void OnDrawUI() = 0;

	protected:
		/**
		 * Implement this method to perform actions on each state update.
		 *
		 * @param deltaTime Time since last call (in seconds).
		 */
		virtual void OnUpdate(double deltaTime) = 0;

		/**
		 * Draws the specified UI element.
		 *
		 * @param shape UI element to draw.
		 * @param shaderProgram Shared pointer to the shader program to use for
		 * rendering the shape.
		 */
		void DrawShape(UI::IShape& shape,
			std::shared_ptr<ShaderProgram> shaderProgram);

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
		 * Returns a shared pointer to the scene stack's event dispatcher.
		 *
		 * @return Shared pointer to the scene stack's event dispatcher.
		 */
		 std::shared_ptr<EventDispatcher> GetSceneStackEventDispatcher() const;

		 /**
		  * Returns a shared pointer to the scene's event dispatcher.
		  *
		  * @return Shared pointer to the scene's event dispatcher.
		  */
		 std::shared_ptr<EventDispatcher> GetEventDispatcher() const;

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
		 * Shared pointer to the scene stack's event dispatcher.
		 */
		std::shared_ptr<EventDispatcher> m_sceneStackEventDispatcher;

		/**
		 * Event dispatcher for the scene.
		 * This will receive window and user input related events.
		 */
		std::shared_ptr<EventDispatcher> m_eventDispatcher;

	private:
		/**
		 * User interface renderer.
		 */
		UI::Renderer m_userInterfaceRenderer;
	};
}

#endif
