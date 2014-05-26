#ifndef IEVENT_H
#define	IEVENT_H

namespace Engine
{
	namespace Event
	{
		class IEvent
		{
		public:
			/**
			 * Constructor.
			 */
			IEvent();

			/**
			 * Destructor.
			 */
			virtual ~IEvent();
		};
	}
}

#endif
