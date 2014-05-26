#ifndef UI_RENDERER_H
#define	UI_RENDERER_H

#include <memory>

#include <Engine/NonCopyable.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/UI/IShape.hpp>

namespace Engine
{
	namespace UI
	{
		class Renderer : private NonCopyable
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param window Shared pointer to the window.
			 * @param resourceManager Shared pointer to the resource manager.
			 */
			Renderer(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager);

			/**
			 * Destructor.
			 */
			~Renderer();

			/**
			 * Renders the specified shape.
			 *
			 * @param shape Reference to the shape to render.
			 * @param shaderProgram Shared pointer to the shader program to use
			 * for rendering the UI element.
			 */
			void RenderShape(UI::IShape& shape, std::shared_ptr<ShaderProgram> shaderProgram);

		private:
			/**
			 * Shared pointer to the window.
			 */
			std::shared_ptr<Window> m_window;

			/**
			 * Shared pointer to the resource manager.
			 */
			std::shared_ptr<ResourceManager> m_resourceManager;
		};
	}
}

#endif
