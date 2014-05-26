#include "Attribute/Tags.hpp"

#include <algorithm>

#include "Event/AddTagEvent.hpp"
#include "Event/RemoveTagEvent.hpp"

namespace Attribute
{
	Tags::Tags(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject)
	: IAttribute(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_tags()
	, m_addTagSubscription(0)
	, m_removeTagSubscription(0)
	{
		// Subscribe for AddTagEvents.
		m_addTagSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::AddTagEvent>(
			[this] (const Event::AddTagEvent& event) {
				AddTag(event.GetTag());
			}
		);

		// Subscribe for RemoveTagEvents.
		m_removeTagSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::RemoveTagEvent>(
			[this] (const Event::RemoveTagEvent& event) {
				RemoveTag(event.GetTag());
			}
		);
	}

	Tags::~Tags()
	{
		// Unsubscribe for AddTagEvents.
		GetGameObjectEventDispatcher()->Unsubscribe<Event::AddTagEvent>(m_addTagSubscription);

		// Unsubscribe for RemoveTagEvents.
		GetGameObjectEventDispatcher()->Unsubscribe<Event::RemoveTagEvent>(m_removeTagSubscription);
	}

	void Tags::AddTag(std::string tag)
	{
		std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
		m_tags.insert(tag);
	}

	void Tags::RemoveTag(std::string tag)
	{
		std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
		auto iter = m_tags.find(tag);
		assert(iter != m_tags.end());
		m_tags.erase(iter);
	}

	bool Tags::HasTag(std::string tag) const
	{
		std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
		return m_tags.find(tag) != m_tags.end();
	}
}
