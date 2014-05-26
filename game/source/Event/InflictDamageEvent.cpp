#include "Event/InflictDamageEvent.hpp"

namespace Event
{
	InflictDamageEvent::InflictDamageEvent(float damage)
	: m_damage(damage)
	{
		// Nothing to do.
	}

	InflictDamageEvent::~InflictDamageEvent()
	{
		// Nothing to do.
	}

	float InflictDamageEvent::GetDamage() const
	{
		return m_damage;
	}
}
