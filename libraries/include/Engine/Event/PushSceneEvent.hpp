#ifndef PUSHSCENEEVENT_H
#define	PUSHSCENEEVENT_H

#include <string>

#include <Engine/Event/IEvent.hpp>

namespace Engine
{
	namespace Event
	{
		class PushSceneEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param sceneName Name for the registered scene to push onto the
			 * top of the scene stack.
			 */
			PushSceneEvent(std::string sceneName);

			/**
			 * Destructor.
			 */
			virtual ~PushSceneEvent();

			/**
			 * Returns the name of the registered scene to push onto the top of
			 * the scene stack.
			 *
			 * @return Name for the registered scene to push onto the top of the
			 * scene stack.
			 */
			std::string GetSceneName() const;

		private:
			/**
			 * Name for the registered scene to push onto the scene stack.
			 */
			std::string m_sceneName;
		};
	}
}

#endif
