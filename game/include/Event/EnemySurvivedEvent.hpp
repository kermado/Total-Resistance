#ifndef ENEMYSURVIVEDEDEVENT_H
#define ENEMYSURVIVEDEDEVENT_H

#include <Engine/Event/IEvent.hpp>

namespace Event
{
	class EnemySurvivedEvent : Engine::Event::IEvent
	{
	public:
		/**
		 * Constructor.
		 */
		EnemySurvivedEvent();

		/**
		 * Destructor.
		 */
		~EnemySurvivedEvent();
	};
}

#endif