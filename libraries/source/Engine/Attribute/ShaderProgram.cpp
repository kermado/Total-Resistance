#include <Engine/Attribute/ShaderProgram.hpp>

namespace Engine
{
	namespace Attribute
	{
		ShaderProgram::ShaderProgram(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject,
				std::string vertexShaderFilepath,
				std::string fragmentShaderFilepath)
		: IAttribute(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
		, m_vertexShaderFilepath(vertexShaderFilepath)
		, m_fragmentShaderFilepath(fragmentShaderFilepath)
		, m_floatUniforms()
		{
			// Nothing to do.
		}

		ShaderProgram::~ShaderProgram()
		{
			// Nothing to do.
		}

		std::string ShaderProgram::GetVertexShaderPath() const
		{
			return m_vertexShaderFilepath;
		}

		std::string ShaderProgram::GetFragmentShaderPath() const
		{
			return m_fragmentShaderFilepath;
		}

		std::shared_ptr<Engine::ShaderProgram> ShaderProgram::GetResource() const
		{
			return GetResourceManager()->GetShaderProgram(m_vertexShaderFilepath, m_fragmentShaderFilepath);
		}

		void ShaderProgram::ApplyUniforms()
		{
			std::shared_ptr<Engine::ShaderProgram> shaderProgram = GetResource();
			if (shaderProgram)
			{
				// Send floating point uniforms to the shader program.
				for (auto iter = m_floatUniforms.begin(); iter != m_floatUniforms.end(); ++iter)
				{
					shaderProgram->SetUniform1f(iter->first, iter->second);
				}
			}
		}

		void ShaderProgram::SetFloat(std::string name, float value)
		{
			m_floatUniforms[name] = value;
		}
	}
}
