#ifndef DESTROYGAMEOBJECTEVENT_H
#define DESTROYGAMEOBJECTEVENT_H

#include <Engine/Event/IEvent.hpp>

namespace Engine
{
	namespace Event
	{
		class DestroyGameObjectEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 */
			DestroyGameObjectEvent();

			/**
			 * Destructor.
			 */
			~DestroyGameObjectEvent();
		};
	}
}

#endif
