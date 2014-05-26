#ifndef RESOURCELOADEDEVENT_H
#define	RESOURCELOADEDEVENT_H

#include <string>
#include <functional>

#include <Engine/Event/IEvent.hpp>
#include <Engine/Model.hpp>

namespace Engine
{
	namespace Event
	{
		template <typename ResourceType>
		class ResourceLoadedEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param name Name of the resource loaded.
			 * @param loadedSuccessfully Was the resource loaded successfully?
			 * @param resource Shared pointer to the loaded resource.
			 * @param callback Callback function.
			 */
			ResourceLoadedEvent(std::string name, bool loadedSuccessfully,
				std::shared_ptr<ResourceType> resource,
				std::function<void(const ResourceLoadedEvent&)> callback)
			: m_name(name)
			, m_loadedSuccessfully(loadedSuccessfully)
			, m_resource(resource)
			, m_callback(callback)
			{
				// Nothing to do.
			}

			/**
			 * Destructor.
			 */
			virtual ~ResourceLoadedEvent()
			{
				// Nothing to do.
			}

			/**
			 * Returns the name of the resource that was loaded.
			 *
			 * @return Name of the resource.
			 */
			std::string GetName() const
			{
				return m_name;
			}

			/**
			 * Returns true if the resource was loaded successfully.
			 *
			 * @return True if the resource was successfully loaded.
			 */
			bool WasSuccessful() const
			{
				return m_loadedSuccessfully;
			}

			/**
			 * Returns a pointer to the loaded resource.
			 *
			 * @return Pointer to the loaded resource.
			 */
			std::shared_ptr<ResourceType> GetResource() const
			{
				return m_resource;
			}

			/**
			 * Returns the callback function that should be executed
			 * once the resource has been completely loaded.
			 */
			std::function<void(const ResourceLoadedEvent&)> GetCallback() const
			{
				return m_callback;
			}

		private:
			/**
			 * Name to identify the resource.
			 */
			std::string m_name;

			/**
			 * Flag that indicates whether the resource was loaded
			 * successfully.
			 */
			bool m_loadedSuccessfully;

			/**
			 * Shared pointer to the loaded resource.
			 */
			std::shared_ptr<ResourceType> m_resource;

			/**
			 * Callback function.
			 */
			std::function<void(const ResourceLoadedEvent&)> m_callback;
		};
	}
}

#endif
