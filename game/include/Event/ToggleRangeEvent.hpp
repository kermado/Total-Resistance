#ifndef TOGGLERANGEEVENT_H
#define TOGGLERANGEEVENT_H

#include <Engine/Event/IEvent.hpp>

namespace Event
{
	class ToggleRangeEvent : public Engine::Event::IEvent
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param visible Whether the range model be made visible.
		 */
		ToggleRangeEvent(bool visible);

		/**
		 * Destructor.
		 */
		~ToggleRangeEvent();

		/**
		 * Returns whether the range model should be made visible.
		 *
		 * @return True if the range model should be made visible.
		 */
		bool GetVisible() const;

	private:
		/**
		 * Whether the range model should be made visible..
		 */
		bool m_visible;
	};
}

#endif
