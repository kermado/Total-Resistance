#ifndef REMOVETAGEVENT_H
#define REMOVETAGEVENT_H

#include <string>

#include <Engine/Event/IEvent.hpp>

namespace Event
{
	class RemoveTagEvent : public Engine::Event::IEvent
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param tag The tag to removed.
		 */
		RemoveTagEvent(std::string tag);

		/**
		 * Destructor.
		 */
		~RemoveTagEvent();

		/**
		 * Returns the tag that was requested to be removed.
		 *
		 * @return Tag to be removed.
		 */
		std::string GetTag() const;

	private:
		/**
		 * The tag to be removed.
		 */
		std::string m_tag;
	};
}

#endif
