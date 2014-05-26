#include <Engine/Event/LoadShaderProgramResourceEvent.hpp>

namespace Engine
{
	namespace Event
	{
		LoadShaderProgramResourceEvent::LoadShaderProgramResourceEvent(std::string name,
			std::string vertexShaderFilepath, std::string fragmentShaderFilepath,
			std::function<void(const ResourceLoadedEvent<ShaderProgram>&)> callback)
		: m_name(name)
		, m_vertexShaderPath(vertexShaderFilepath)
		, m_fragmentShaderPath(fragmentShaderFilepath)
		, m_callback(callback)
		{
			// Nothing to do.
		}

		LoadShaderProgramResourceEvent::~LoadShaderProgramResourceEvent()
		{
			// Nothing to do.
		}

		std::string LoadShaderProgramResourceEvent::GetName() const
		{
			return m_name;
		}

		std::string LoadShaderProgramResourceEvent::GetVertexShaderPath() const
		{
			return m_vertexShaderPath;
		}

		std::string LoadShaderProgramResourceEvent::GetFragmentShaderPath() const
		{
			return m_fragmentShaderPath;
		}

		std::function<void(const ResourceLoadedEvent<ShaderProgram>&)> LoadShaderProgramResourceEvent::GetCallback() const
		{
			return m_callback;
		}
	}
}
