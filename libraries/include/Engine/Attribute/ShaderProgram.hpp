#ifndef SHADERPROGRAMATTRIBUTE_H
#define	SHADERPROGRAMATTRIBUTE_H

#include <string>
#include <memory>

#include <Engine/Attribute/IAttribute.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>

namespace Engine
{
	namespace Attribute
	{
		class ShaderProgram : public IAttribute
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
			 * attribute is attached to.
			 * @param vertexShaderFilepath Path to the vertex shader.
			 * @param fragmentShaderFilepath Path to the fragment shader.
			 */
			ShaderProgram(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject,
				std::string vertexShaderFilepath,
				std::string fragmentShaderFilepath);

			/**
			 * Destructor.
			 */
			~ShaderProgram();

			/**
			 * Returns the path to the vertex shader.
			 *
			 * @return Path to the vertex shader.
			 */
			std::string GetVertexShaderPath() const;

			/**
			 * Returns the path to the fragment shader.
			 *
			 * @return Path to the fragment shader.
			 */
			std::string GetFragmentShaderPath() const;

			/**
			 * Returns a shared pointer to the shader program resource.
			 *
			 * @return Shared pointer to the shader program resource.
			 */
			std::shared_ptr<Engine::ShaderProgram> GetResource() const;

			/**
			 * Applies all of the uniforms to the shader program
			 * resource.
			 */
			void ApplyUniforms();

			/**
			 * Sets a named float value in the shader.
			 *
			 * @param name Name of the float uniform in the shader.
			 * @param value Value for the uniform.
			 */
			void SetFloat(std::string name, float value);

		private:
			/**
			 * Path to the vertex shader.
			 */
			std::string m_vertexShaderFilepath;

			/**
			 * Path to the fragment shader.
			 */
			std::string m_fragmentShaderFilepath;

			/**
			 * Named floating point values to be set in the shader.
			 */
			std::unordered_map<std::string, float> m_floatUniforms;
		};
	}
}

#endif
