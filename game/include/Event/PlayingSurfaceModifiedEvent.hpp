#ifndef PLAYINGSURFACEMODIFIEDEVENT_H
#define PLAYINGSURFACEMODIFIEDEVENT_H

#include <Engine/Event/IEvent.hpp>

#include "PlayingSurface.hpp"

namespace Event
{
	class PlayingSurfaceModifiedEvent : public Engine::Event::IEvent
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param playingSurface The new playing surface.
		 */
		PlayingSurfaceModifiedEvent(const PlayingSurface& playingSurface);

		/**
		 * Destructor.
		 */
		~PlayingSurfaceModifiedEvent();

		/**
		 * Returns a const reference to a copy of the new playing surface.
		 *
		 * @return Copy of the playing surface.
		 */
		const PlayingSurface& GetPlayingSurface() const;

	private:
		/**
		 * Copy of the new playing surface.
		 */
		const PlayingSurface m_playingSurface;
	};
}

#endif
