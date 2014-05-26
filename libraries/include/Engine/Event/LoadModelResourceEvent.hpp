#ifndef LOADMODELRESOURCEEVENT_H
#define	LOADMODELRESOURCEEVENT_H

#include <string>
#include <functional>

#include <Engine/Event/IEvent.hpp>
#include <Engine/Event/ResourceLoadedEvent.hpp>

namespace Engine
{
	namespace Event
	{
		class LoadModelResourceEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param name Name used to identify the resource.
			 * @param filepath Filepath Path to the model file.
			 * @param callback Callback function.
			 */
			LoadModelResourceEvent(std::string name, std::string filepath,
				std::function<void(const ResourceLoadedEvent<Model>&)> callback);

			/**
			 * Destructor.
			 */
			virtual ~LoadModelResourceEvent();

			/**
			 * Returns the name that should identify the resource.
			 *
			 * @return Name for the resource.
			 */
			std::string GetName() const;

			/**
			 * Returns the path to the model file.
			 *
			 * @return Path to the model file.
			 */
			std::string GetPath() const;

			/**
			 * Returns the callback function that should be executed
			 * once the resource has been completely loaded.
			 */
			std::function<void(const ResourceLoadedEvent<Model>&)> GetCallback() const;

		private:
			/**
			 * Name to identify the resource.
			 */
			std::string m_name;

			/**
			 * Path to the model file.
			 */
			std::string m_filepath;

			/**
			 * Callback function.
			 */
			std::function<void(const ResourceLoadedEvent<Model>&)> m_callback;
		};
	}
}

#endif
