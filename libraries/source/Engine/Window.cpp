#include <Engine/Window.hpp>

#include <iostream>

#include <Engine/Event/KeyPressedEvent.hpp>
#include <Engine/Event/MouseButtonPressedEvent.hpp>
#include <Engine/Event/WindowResizeEvent.hpp>

namespace Engine
{
	Window::Window(int width, int height, std::string title,
		std::shared_ptr<Window> accessResourcesFromContext)
	: m_eventDispatcher(std::make_shared<EventDispatcher>())
	{
		// Window should be invisible.
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

		// Anti-aliasing.
		glfwWindowHint(GLFW_SAMPLES, 16);

		// Request suitable OpenGL context.
		#ifdef __APPLE__
			// Request OpenGL 3.2 context on mac OSX.
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#elif __linux
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		#else
			// Sorry, only OSX and Linux have been tested.
			exit(1);
		#endif

		// Get the share window.
		GLFWwindow* shareWindow = NULL;
		if (accessResourcesFromContext)
		{
			shareWindow = accessResourcesFromContext->GetWindow();
		}

		// Create the window.
		m_window = glfwCreateWindow(
			width,
			height,
			title.c_str(),
			NULL, // Monitor
			shareWindow
		);

		// Ensure that the window was created successfully.
		if (!m_window)
		{
			std::cerr << "Error: Failed to open a window" << std::endl;
			exit(1);
		}

		// Set the window's user defined pointer to point to this instance.
		// This allows us to call member functions on this Window instance.
		glfwSetWindowUserPointer(m_window, this);

		// Set callbacks.
		glfwSetErrorCallback(&Window::ErrorCallback);
		glfwSetKeyCallback(m_window, &Window::KeyCallback);
		glfwSetMouseButtonCallback(m_window, &Window::MouseButtonCallback);
		glfwSetWindowSizeCallback(m_window, &Window::WindowSizeCallback);
	}

	Window::Window(int width, int height, std::string title)
	: Window(width, height, title, nullptr)
	{
		// Nothing to do.
	}

	Window::~Window()
	{
		// Destroy the GLFW window.
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}

	void Window::Update()
	{
		m_eventDispatcher->Update();
	}

	void Window::MakeContextCurrent()
	{
		glfwMakeContextCurrent(m_window);
	}

	void Window::Show()
	{
		glfwShowWindow(m_window);
	}

	void Window::Hide()
	{
		glfwHideWindow(m_window);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	bool Window::ShouldClose() const
	{
		return static_cast<bool>(glfwWindowShouldClose(m_window));
	}

	glm::vec2 Window::GetDimensions() const
	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);
		return glm::vec2(width, height);
	}

	int Window::GetWidth() const
	{
		int width;
		glfwGetWindowSize(m_window, &width, NULL);
		return width;
	}

	int Window::GetHeight() const
	{
		int height;
		glfwGetWindowSize(m_window, NULL, &height);
		return height;
	}

	glm::vec2 Window::GetCursorPosition() const
	{
		double x, y;
		glfwGetCursorPos(m_window, &x, &y);
		return glm::vec2(x, y);
	}

	std::shared_ptr<EventDispatcher> Window::GetEventDispatcher()
	{
		return m_eventDispatcher;
	}

	GLFWwindow* Window::GetWindow()
	{
		return m_window;
	}

	void Window::ErrorCallback(int error, const char* description)
	{
		std::cout << "GLFW ERROR: " << description << std::endl;
		exit(1); // Critical error!
	}

	void Window::KeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
	{
		// Get the pointer to the Window instance.
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

		// Raise the relevant event.
		switch (action)
		{
			case GLFW_PRESS:
				window->GetEventDispatcher()->Enqueue<Event::KeyPressedEvent>(key);
				break;
		}
	}

	void Window::MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
	{
		// Get the pointer to the Window instance.
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

		// Raise the relevant event.
		switch (action)
		{
			case GLFW_PRESS:
				window->GetEventDispatcher()->Enqueue<Event::MouseButtonPressedEvent>(button, window->GetCursorPosition());
				break;
		}
	}

	void Window::WindowSizeCallback(GLFWwindow* glfwWindow, int width, int height)
	{
		// Get the pointer to the Window instance.
		Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

		// Raise the relevant event.
		window->GetEventDispatcher()->Enqueue<Event::WindowResizeEvent>(width, height);
	}
}
