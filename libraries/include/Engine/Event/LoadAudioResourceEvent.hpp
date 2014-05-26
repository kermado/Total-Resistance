#ifndef LOADAUDIORESOURCEEVENT_H
#define LOADAUDIORESOURCEEVENT_H

#include <string>

#include <Engine/IAudioSource.hpp>

#include <Engine/Event/IEvent.hpp>
#include <Engine/Event/ResourceLoadedEvent.hpp>

namespace Engine
{
	namespace Event
	{
		class LoadAudioResourceEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param name Name used to identify the resource.
			 * @param filepath Filepath Path to the audio file.
			 * @param callback Callback function.
			 */
			LoadAudioResourceEvent(std::string name,
				std::string filepath,
				std::function<void(const ResourceLoadedEvent<IAudioSource>&)> callback);

			/**
			 * Destructor.
			 */
			virtual ~LoadAudioResourceEvent();

			/**
			 * Returns the name that should identify the resource.
			 *
			 * @return Name for the resource.
			 */
			std::string GetName() const;

			/**
			 * Returns the path to the audio file.
			 *
			 * @return Path to the audio file.
			 */
			std::string GetPath() const;

			/**
			 * Returns the callback function that should be executed
			 * once the resource has been completely loaded.
			 */
			std::function<void(const ResourceLoadedEvent<IAudioSource>&)> GetCallback() const;

		private:
			/**
			 * The resource name.
			 */
			std::string m_name;

			/**
			 * The path to the file.
			 */
			std::string m_filepath;

			/**
			 * The callback function.
			 */
			std::function<void(const ResourceLoadedEvent<IAudioSource>&)> m_callback;
		};
	}
}

#endif
