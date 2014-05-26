#include <Engine/Event/LoadModelResourceEvent.hpp>

namespace Engine
{
	namespace Event
	{
		LoadModelResourceEvent::LoadModelResourceEvent(std::string name,
			std::string filepath, std::function<void(const ResourceLoadedEvent<Model>&)> callback)
		: m_name(name)
		, m_filepath(filepath)
		, m_callback(callback)
		{
			// Nothing to do.
		}

		LoadModelResourceEvent::~LoadModelResourceEvent()
		{
			// Nothing to do.
		}

		std::string LoadModelResourceEvent::GetName() const
		{
			return m_name;
		}

		std::string LoadModelResourceEvent::GetPath() const
		{
			return m_filepath;
		}

		std::function<void(const ResourceLoadedEvent<Model>&)> LoadModelResourceEvent::GetCallback() const
		{
			return m_callback;
		}
	}
}
