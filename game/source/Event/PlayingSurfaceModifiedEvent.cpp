#include "Event/PlayingSurfaceModifiedEvent.hpp"

namespace Event
{
	PlayingSurfaceModifiedEvent::PlayingSurfaceModifiedEvent(const PlayingSurface& playingSurface)
	: IEvent()
	, m_playingSurface(playingSurface)
	{
		// Nothing to do.
	}

	PlayingSurfaceModifiedEvent::~PlayingSurfaceModifiedEvent()
	{
		// Nothing to do.
	}

	const PlayingSurface& PlayingSurfaceModifiedEvent::GetPlayingSurface() const
	{
		return m_playingSurface;
	}
}
