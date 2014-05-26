#include "Event/AddTagEvent.hpp"

namespace Event
{
	AddTagEvent::AddTagEvent(std::string tag)
	: m_tag(tag)
	{
		// Nothing to do.
	}

	AddTagEvent::~AddTagEvent()
	{
		// Nothing to do.
	}

	std::string AddTagEvent::GetTag() const
	{
		return m_tag;
	}
}
