#include <iostream>

#include <Engine/Application.hpp>
#include <Engine/Event/KeyPressedEvent.hpp>
#include <Engine/Event/MouseButtonPressedEvent.hpp>
#include <Engine/Event/PushSceneEvent.hpp>
#include <Engine/Event/WindowResizeEvent.hpp>

namespace Engine
{
	Application::Application(unsigned int width, unsigned int height, std::string title)
	: m_running(false)
	{
		// Initialize GLFW.
		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW" << std::endl;
			exit(1); // Critical failure!
		}

		// Create a window for the loading thread.
		std::shared_ptr<Window> loadingWindow(new Window(1, 1, "Loading Window"));

		// Create the main window for the application and share the loading
		// thread window's OpenGL context with the main thread window's OpenGL
		// context.
		m_mainWindow = std::shared_ptr<Window>(new Window(width, height, title, loadingWindow));

		// Make the main window's OpenGL context current to the main thread.
		m_mainWindow->MakeContextCurrent();

		// Set the viewport size.
		glViewport(0, 0, width, height);

		// Resize the viewport when the window is resized.
		m_mainWindow->GetEventDispatcher()->Subscribe<Event::WindowResizeEvent>([](const Event::WindowResizeEvent& event) {
			glViewport(0, 0, event.GetWidth(), event.GetHeight());
		});

		// Set the blending function to use alpha blending.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Initialize GLEW.
		glewExperimental = GL_TRUE; // OSX fix.
		if (glewInit() != GLEW_OK)
		{
			exit(1); // Critical failure!
		}

		// Create the resource manager.
		m_resourceManager = std::shared_ptr<ResourceManager>(new ResourceManager(loadingWindow));

		// Create the scene stack.
		m_sceneStack = std::unique_ptr<SceneStack>(new SceneStack(m_mainWindow, m_resourceManager));

		// TODO: Remove this.
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Shading Lanugage Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	}

	Application::~Application()
	{
		// Terminate GLFW.
		glfwTerminate();
	}

	void Application::PushScene(std::string name)
	{
		// We call the scene stack's PushSceneEvent handler directly.
		const Event::PushSceneEvent event(name);
		m_sceneStack->HandlePushSceneEvent(event);
	}

	void Application::Run()
	{
		if (!m_running)
		{
			// Main loop is now running.
			m_running = true;

			// Display the main window.
			m_mainWindow->Show();

			// Holds the time at which the previous state update was performed.
			double lastUpdate = glfwGetTime();

			// Time per state update frame (seconds).
			const double timePerFrame = 1.0 / 60.0;

			// Holds the time since the last update.
			double timeSinceLastUpdate = 0;

			// Maximum number of state updates to perform before rendering.
			const int maxUpdatesPerRender = 2;
			int updateCount = 0;

			// Game loop.
			while (!m_mainWindow->ShouldClose())
			{
				// Time now.
				const double now = glfwGetTime();

				// Time since last iteration.
				timeSinceLastUpdate += now - lastUpdate;
				lastUpdate = now;

				while (timeSinceLastUpdate >= timePerFrame && updateCount < maxUpdatesPerRender)
				{
					timeSinceLastUpdate -= timePerFrame;
					glfwPollEvents();
					m_mainWindow->Update();
					m_resourceManager->Update();
					m_sceneStack->Update(timePerFrame);
					++updateCount;
				}

				updateCount = 0;
				Render();
			}

			// Hide the main window.
			m_mainWindow->Hide();

			// Main loop is now not running.
			m_running = false;
		}
		else
		{
			assert(0);
		}
	}

	void Application::Render()
	{
		// Clear the depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Enable Z-buffer reading, writing and testing.
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		// Disable blending.
		glDisable(GL_BLEND);

		// Perform OpenGL game rendering.
		m_sceneStack->Render();

		// Clear the depth buffer so that we don't hide the UI layer behind the
		// 3D scene.
		glClear(GL_DEPTH_BUFFER_BIT);

		// Disable Z-buffer reading, writing and testing.
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);

		// Enable blending.
		glEnable(GL_BLEND);

		// Draw UI.
		m_sceneStack->DrawUI();

		// Swap buffers to display the rendered frame.
		m_mainWindow->SwapBuffers();
	}
}
