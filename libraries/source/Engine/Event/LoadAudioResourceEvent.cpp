#include <Engine/Event/LoadAudioResourceEvent.hpp>

namespace Engine
{
	namespace Event
	{
		LoadAudioResourceEvent::LoadAudioResourceEvent(std::string name,
			std::string filepath,
			std::function<void(const ResourceLoadedEvent<IAudioSource>&)> callback)
		: m_name(name)
		, m_filepath(filepath)
		, m_callback(callback)
		{
			// Nothing to do.
		}

		LoadAudioResourceEvent::~LoadAudioResourceEvent()
		{
			// Nothing to do.
		}

		std::string LoadAudioResourceEvent::GetName() const
		{
			return m_name;
		}

		std::string LoadAudioResourceEvent::GetPath() const
		{
			return m_filepath;
		}

		std::function<void(const ResourceLoadedEvent<IAudioSource>&)> LoadAudioResourceEvent::GetCallback() const
		{
			return m_callback;
		}
	}
}
