#ifndef TAGS_H
#define TAGS_H

#include <memory>
#include <string>
#include <set>

#include <Engine/Attribute/IAttribute.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>

namespace Attribute
{
	class Tags : public Engine::Attribute::IAttribute
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param window Shared pointer to the window.
		 * @param resourceManager Shared pointer to the resource manager.
		 * @param sceneEventDispatcher Shared pointer to the scene's Event
		 * Dispatcher.
		 * @param gameObjectEventDispatcher Shared pointer to the Game
		 * Object's Event Dispatcher.
		 * @param gameObject Weak pointer to the Game Object that the
		 * attribute is attached to.
		 */
		Tags(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject);

		/**
		 * Destructor.
		 */
		~Tags();

		/**
		 * Adds the specified tag.
		 *
		 * @param tag Tag to add.
		 */
		void AddTag(std::string tag);

		/**
		 * Removes the specified tag.
		 *
		 * @param tag Tag to remove.
		 */
		void RemoveTag(std::string tag);

		/**
		 * Returns true if the tag was added.
		 *
		 * @note Tags are case insensitive.
		 *
		 * @param tag Tag to check for presence of.
		 * @return True if the specified tag was found.
		 */
		bool HasTag(std::string tag) const;

	private:
		/**
		 * The set of tags that have been added.
		 */
		std::set<std::string> m_tags;

		/**
		 * Subscription identifier for the AddTagEvent subscription.
		 */
		Engine::EventDispatcher::SubscriptionID m_addTagSubscription;

		/**
		 * Subscription identifier for the RemoveTagEvent subscription.
		 */
		Engine::EventDispatcher::SubscriptionID m_removeTagSubscription;
	};
}

#endif
