#ifndef ADDTAGEVENT_H
#define ADDTAGEVENT_H

#include <string>

#include <Engine/Event/IEvent.hpp>

namespace Event
{
	class AddTagEvent : public Engine::Event::IEvent
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param tag The tag to add.
		 */
		AddTagEvent(std::string tag);

		/**
		 * Destructor.
		 */
		~AddTagEvent();

		/**
		 * Returns the tag that was requested to be added.
		 *
		 * @return Tag to be added.
		 */
		std::string GetTag() const;

	private:
		/**
		 * The tag to be added.
		 */
		std::string m_tag;
	};
}

#endif
