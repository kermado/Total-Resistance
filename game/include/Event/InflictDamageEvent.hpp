#ifndef INFLICTDAMAGEVENT_H
#define INFLICTDAMAGEVENT_H

#include <Engine/Event/IEvent.hpp>

namespace Event
{
	class InflictDamageEvent : public Engine::Event::IEvent
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param damage Amount of damage to inflict.
		 */
		InflictDamageEvent(float damage);

		/**
		 * Destructor.
		 */
		~InflictDamageEvent();

		/**
		 * Returns the amount of damage to be inflicted.
		 *
		 * @return Amount of damage to inflict.
		 */
		float GetDamage() const;

	private:
		/**
		 * The amount of damage to be inflicted.
		 */
		float m_damage;
	};
}

#endif
