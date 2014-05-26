#ifndef LOADTEXTURERESOURCEEVENT_H
#define LOADTEXTURERESOURCEEVENT_H

#include <string>

#include <Engine/Event/IEvent.hpp>
#include <Engine/Event/ResourceLoadedEvent.hpp>
#include <Engine/Texture.hpp>

namespace Engine
{
	namespace Event
	{
		class LoadTextureResourceEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param name Name used to identify the resource.
			 * @param filepath Filepath Path to the texture file.
			 * @param callback Callback function.
			 */
			LoadTextureResourceEvent(std::string name,
				std::string filepath,
				std::function<void(const ResourceLoadedEvent<Texture>&)> callback);

			/**
			 * Destructor.
			 */
			virtual ~LoadTextureResourceEvent();

			/**
			 * Returns the name that should identify the resource.
			 *
			 * @return Name for the resource.
			 */
			std::string GetName() const;

			/**
			 * Returns the path to the texture file.
			 *
			 * @return Path to the texture file.
			 */
			std::string GetPath() const;

			/**
			 * Returns the callback function that should be executed
			 * once the resource has been completely loaded.
			 */
			std::function<void(const ResourceLoadedEvent<Texture>&)> GetCallback() const;

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
			std::function<void(const ResourceLoadedEvent<Texture>&)> m_callback;
		};
	}
}

#endif
