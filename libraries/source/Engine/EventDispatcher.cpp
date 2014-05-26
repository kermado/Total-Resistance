#include <Engine/EventDispatcher.hpp>

namespace Engine
{
	EventDispatcher::EventDispatcher()
	: m_nextSubscriptionId(1)
	, m_dispatchers()
	, m_eventOrder()
	{
		// Nothing to do.
	}

	EventDispatcher::~EventDispatcher()
	{
		// Nothing to do.
	}
}
