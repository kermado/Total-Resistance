#ifndef LOADSHADERPROGRAMRESOURCEEVENT_H
#define	LOADSHADERPROGRAMRESOURCEEVENT_H

#include <string>
#include <functional>

#include <Engine/Event/IEvent.hpp>
#include <Engine/Event/ResourceLoadedEvent.hpp>
#include <Engine/ShaderProgram.hpp>

namespace Engine
{
	namespace Event
	{
		class LoadShaderProgramResourceEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param name Name used to identify the resource.
			 * @param vertexShaderFilepath Path to the vertex shader.
			 * @param fragmentShaderFilepath Path to the fragment shader.
			 * @param callback Callback function.
			 */
			LoadShaderProgramResourceEvent(const std::string name,
				std::string vertexShaderFilepath, std::string fragmentShaderFilepath,
				std::function<void(const ResourceLoadedEvent<ShaderProgram>&)> callback);

			/**
			 * Destructor.
			 */
			virtual ~LoadShaderProgramResourceEvent();

			/**
			 * Returns the name that should identify the resource.
			 *
			 * @return Name for the resource.
			 */
			std::string GetName() const;

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
			 * Returns the callback function that should be executed
			 * once the resource has been completely loaded.
			 */
			std::function<void(const ResourceLoadedEvent<ShaderProgram>&)> GetCallback() const;

		private:
			/**
			 * Name to identify the resource.
			 */
			std::string m_name;

			/**
			 * Path to the vertex shader.
			 */
			std::string m_vertexShaderPath;

			/**
			 * Path to the fragment shader.
			 */
			std::string m_fragmentShaderPath;

			/**
			 * Callback function.
			 */
			std::function<void(const ResourceLoadedEvent<ShaderProgram>&)> m_callback;
		};
	}
}

#endif
