#include "Event/RemoveTagEvent.hpp"

namespace Event
{
	RemoveTagEvent::RemoveTagEvent(std::string tag)
	: m_tag(tag)
	{
		// Nothing to do.
	}

	RemoveTagEvent::~RemoveTagEvent()
	{
		// Nothing to do.
	}

	std::string RemoveTagEvent::GetTag() const
	{
		return m_tag;
	}
}
