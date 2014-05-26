#include <Engine/Event/LoadTextureResourceEvent.hpp>

namespace Engine
{
	namespace Event
	{
		LoadTextureResourceEvent::LoadTextureResourceEvent(std::string name,
			std::string filepath,
			std::function<void(const ResourceLoadedEvent<Texture>&)> callback)
		: m_name(name)
		, m_filepath(filepath)
		, m_callback(callback)
		{
			// Nothing to do.
		}

		LoadTextureResourceEvent::~LoadTextureResourceEvent()
		{
			// Nothing to do.
		}

		std::string LoadTextureResourceEvent::GetName() const
		{
			return m_name;
		}

		std::string LoadTextureResourceEvent::GetPath() const
		{
			return m_filepath;
		}

		std::function<void(const ResourceLoadedEvent<Texture>&)> LoadTextureResourceEvent::GetCallback() const
		{
			return m_callback;
		}
	}
}
