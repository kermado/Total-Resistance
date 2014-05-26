#include "Event/ToggleRangeEvent.hpp"

namespace Event
{
	ToggleRangeEvent::ToggleRangeEvent(bool visible)
	: m_visible(visible)
	{
		// Nothing to do.
	}

	ToggleRangeEvent::~ToggleRangeEvent()
	{
		// Nothing to do.
	}

	bool ToggleRangeEvent::GetVisible() const
	{
		return m_visible;
	}
}
