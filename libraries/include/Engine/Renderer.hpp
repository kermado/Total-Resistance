#ifndef RENDERER_H
#define	RENDERER_H

#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Engine/NonCopyable.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Model.hpp>
#include <Engine/ShaderProgram.hpp>

namespace Engine
{
	class Renderer : private NonCopyable
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param resourceManager Shared pointer to the resource manager.
		 */
		Renderer(std::shared_ptr<ResourceManager> resourceManager);

		/**
		 * Destructor.
		 */
		~Renderer();

		/**
		 * Returns the number of draw calls performed to render the last frame.
		 *
		 * @return Number of draw calls made to render the last frame.
		 */
		unsigned int GetDrawCount() const;

		/**
		 * Renders the specified game scene.
		 *
		 * @param gameObjects Reference to the game objects list.
		 * @param cameraGameObject Shared pointer to the camera game object that
		 * should be used for rendering.
		 */
		void Render(std::map<GameObject::ID, std::shared_ptr<GameObject>>& gameObjects,
			std::shared_ptr<GameObject> cameraGameObject);

	private:
		/**
		 * Populates the render list by determining whether the specified
		 * Game Object should be rendered and then recursively checks any child
		 * Game Objects.
		 *
		 * @param gameObject Shared pointer to the Game Object to check whether
		 * it should be added to the render list.
		 */
		void PopulateRenderList(std::shared_ptr<GameObject> gameObject);

		/**
		 * Renders the specified game object.
		 *
		 * @param gameObject Shared pointer to the game object that should be
		 * rendered.
		 * @param projectionMatrix The projection matrix.
		 * @param viewMatrix The view matrix.
		 * @param directionalLights Vector of shared pointers to directional
		 * light game objects.
		 */
		void RenderGameObject(std::shared_ptr<GameObject> gameObject,
			const glm::mat4& projectionMatrix,
			const glm::mat4& viewMatrix,
			const std::vector<std::shared_ptr<GameObject>>& directionalLights);

		/**
		 * Recursively renders a node and all of its children.
		 *
		 * @param node Pointer to the top node to start rendering.
		 * @param parentAccumulatedTransformation Transformation matrix
		 * for the specified node's parent, which takes a vector from the parent
		 * node space to the model space.
		 * @param animationTime Time after the start of the animation to render.
		 * @param shaderProgram Shared pointer to the shader program to use for
		 * rendering the node.
		 */
		void RenderNode(std::shared_ptr<Model::Node> node,
			const glm::mat4& parentAccumulatedTransformation,
			double animationTime,
			std::shared_ptr<ShaderProgram> shaderProgram);

	private:
		/**
		 * Shared pointer to the resource manager.
		 */
		std::shared_ptr<ResourceManager> m_resourceManager;

		/**
		 * Holds shared pointers to all of the Game Objects that need to be
		 * rendered.
		 */
		std::vector<std::shared_ptr<GameObject>> m_renderList;

		/**
		 * The shader program currently being used.
		 */
		std::shared_ptr<ShaderProgram> m_currentShaderProgram;

		/**
		 * Counter for the number of draw calls performed.
		 */
		unsigned int m_drawCount;
	};
}

#endif
