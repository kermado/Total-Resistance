#ifndef POPSCENEEVENT_H
#define	POPSCENEEVENT_H

#include <string>

#include <Engine/Event/IEvent.hpp>

namespace Engine
{
	namespace Event
	{
		class PopSceneEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 */
			PopSceneEvent();

			/**
			 * Destructor.
			 */
			virtual ~PopSceneEvent();
		};
	}
}

#endif
