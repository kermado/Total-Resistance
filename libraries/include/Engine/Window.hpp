#ifndef WINDOW_H
#define	WINDOW_H

#include <string>
#include <memory>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Engine/NonCopyable.hpp>
#include <Engine/EventDispatcher.hpp>

namespace Engine
{
	/**
	 * Provides a nice C++11 interface to GLFW.
	 *
	 * @note Not all GLFW function calls have been wrapped.
	 */
	class Window : private NonCopyable
	{
	public:
		/**
		 * Constructor.
		 * Creates a hidden window with the specified dimensions and able to
		 * use resources loaded in the OpenGL context of the specified window.
		 *
		 * @param width Width for the window (in pixels).
		 * @param height Height for the window (in pixels).
		 * @param title Title for the window.
		 * @param accessResourcesFromContext Shared pointer to the window that
		 * this window should be able to access resources from.
		 */
		Window(int width, int height, std::string title,
			std::shared_ptr<Window> accessResourcesFromContext);

		/**
		 * Constructor.
		 * Creates a hidden window with the specified dimensions.
		 *
		 * @param width Width for the window (in pixels).
		 * @param height Height for the window (in pixels).
		 * @param title Title for the window.
		 */
		Window(int width, int height, std::string title);

		/**
		 * Destructor.
		 */
		~Window();

		/**
		 * Processes the window's event dispatcher.
		 */
		void Update();

		/**
		 * Makes the window's OpenGL context current to the thread of execution.
		 */
		void MakeContextCurrent();

		/**
		 * Makes the window visible.
		 */
		void Show();

		/**
		 * Makes the window invisible.
		 */
		void Hide();

		/**
		 * Processes events, causing callbacks to be triggered.
		 */
		static void PollEvents();

		/**
		 * Swaps the window's front and back buffers.
		 */
		void SwapBuffers();

		/**
		 * Returns true if window has been requested to be closed.
		 *
		 * @return True if the window should be closed.
		 */
		bool ShouldClose() const;

		/**
		 * Returns the window dimensions.
		 *
		 * @return Window dimensions (in pixels).
		 */
		glm::vec2 GetDimensions() const;

		/**
		 * Returns the window's width (in pixels).
		 *
		 * @return Window width (pixels).
		 */
		int GetWidth() const;

		/**
		 * Returns the window's height (in pixels).
		 *
		 * @return Window height (pixels).
		 */
		int GetHeight() const;

		/**
		 * Returns the current viewport coordinates of the mouse cursor.
		 *
		 * @return Mouse cursor coordinates.
		 */
		glm::vec2 GetCursorPosition() const;

		/**
		 * Returns a shared pointer to the window's event dispatcher.
		 *
		 * @return Shared pointer to the window's event dispatcher.
		 */
		std::shared_ptr<EventDispatcher> GetEventDispatcher();

	private:
		/**
		 * Returns the internal GLFW window pointer.
		 *
		 * @return Internal GLFW window pointer.
		 */
		GLFWwindow* GetWindow();

		/**
		 * GLFW error callback function.
		 *
		 * @param error Error code.
		 * @param description Description for the error that occured.
		 */
		static void ErrorCallback(int error, const char* description);

		/**
		 * GLFW key callback function.
		 *
		 * @param glfwWindow Pointer to the GLFW window whose callback to set.
		 * @param key The keyboard key that reported the action.
		 * @param scancode System specific scancode of the key.
		 * @param action The action that was performed.
		 * @param mods Bit field describing which modifier keys were held down.
		 */
		static void KeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);

		/**
		 * GLFW mouse button callback function.
		 *
		 * @param glfwWindow Pointer to the GLFW window whose callback to set.
		 * @param button The mouse button that the action was performed on.
		 * @param action The action that was performed.
		 * @param mods Bit field describing which modifier keys were held down.
		 */
		static void MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);

		/**
		 * GLFW window size callback function.
		 *
		 * @param glfwWindow Pointer to the GLFW window whose callback to set.
		 * @param width The new width, in screen coordinates, of the window.
		 * @param height The new height, in screen coordinates, of the window.
		 */
		static void WindowSizeCallback(GLFWwindow* glfwWindow, int width, int height);

	private:
		/**
		 * Pointer to the GLFW window being wrapped.
		 */
		GLFWwindow* m_window;

		/**
		 * Shared pointer to the window's event dispatcher.
		 */
		std::shared_ptr<EventDispatcher> m_eventDispatcher;
	};
};

#endif
