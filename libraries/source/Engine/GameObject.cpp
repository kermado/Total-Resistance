#include <Engine/GameObject.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>

#include <Engine/IGameScene.hpp>
#include <Engine/Event/ChildGameObjectAttachedEvent.hpp>

namespace Engine
{
	// Initialize the next identifier to be used to identify Game Objects.
	GameObject::ID GameObject::s_nextIdentifier = GameObject::ID(0);

	GameObject::GameObject(std::shared_ptr<Window> window,
		std::shared_ptr<ResourceManager> resourceManager,
		std::shared_ptr<EventDispatcher> sceneEventDispatcher)
	: m_window(window)
	, m_resourceManager(resourceManager)
	, m_sceneEventDispatcher(sceneEventDispatcher)
	, m_eventDispatcher(std::make_shared<EventDispatcher>())
	, m_identifier(s_nextIdentifier++)
	, m_dead(false)
	, m_children()
	, m_attributes()
	, m_behaviours()
	, m_createGameObjectSubscription(0)
	, m_destroyGameObjectSubscription(0)
	{
		// Subscribe for create game object events.
		m_createGameObjectSubscription = m_eventDispatcher->Subscribe<Event::CreateGameObjectEvent>(
			[this] (const Event::CreateGameObjectEvent& event) {
				std::shared_ptr<const IGameObjectFactory> factory = event.GetFactory();
				if (factory)
				{
					event.ExecuteCallback(CreateGameObject(*factory));
				}
				else
				{
					event.ExecuteCallback(CreateGameObject());
				}
			}
		);

		// Subscribe for destroy game object events.
		m_destroyGameObjectSubscription = m_eventDispatcher->Subscribe<Event::DestroyGameObjectEvent>(
			[this] (const Event::DestroyGameObjectEvent& event) {
				m_dead = true;
			}
		);
	}

	GameObject::GameObject(std::shared_ptr<Window> window,
		std::shared_ptr<ResourceManager> resourceManager,
		std::shared_ptr<EventDispatcher> sceneEventDispatcher,
		std::weak_ptr<GameObject> parent)
	: m_window(window)
	, m_resourceManager(resourceManager)
	, m_sceneEventDispatcher(sceneEventDispatcher)
	, m_eventDispatcher(std::make_shared<EventDispatcher>())
	, m_identifier(s_nextIdentifier++)
	, m_dead(false)
	, m_children()
	, m_attributes()
	, m_behaviours()
	, m_createGameObjectSubscription(0)
	, m_destroyGameObjectSubscription(0)
	{
		// Subscribe for create game object events.
		m_createGameObjectSubscription = m_eventDispatcher->Subscribe<Event::CreateGameObjectEvent>(
			[this] (const Event::CreateGameObjectEvent& event) {
				std::shared_ptr<const IGameObjectFactory> factory = event.GetFactory();
				if (factory)
				{
					event.ExecuteCallback(CreateGameObject(*factory));
				}
				else
				{
					event.ExecuteCallback(CreateGameObject());
				}
			}
		);

		// Subscribe for destroy game object events.
		m_destroyGameObjectSubscription = m_eventDispatcher->Subscribe<Event::DestroyGameObjectEvent>(
			[this] (const Event::DestroyGameObjectEvent& event) {
				m_dead = true;
			}
		);
	}

	GameObject::~GameObject()
	{
		// Unsubscribe for create game object events.
		m_eventDispatcher->Unsubscribe<Event::CreateGameObjectEvent>(m_createGameObjectSubscription);

		// Unsubscribe for destroy game object events.
		m_eventDispatcher->Unsubscribe<Event::DestroyGameObjectEvent>(m_destroyGameObjectSubscription);
	}

	GameObject::ID GameObject::GetId() const
	{
		return m_identifier;
	}

	bool GameObject::IsDead() const
	{
		return m_dead;
	}

	void GameObject::AddChild(std::shared_ptr<GameObject> child)
	{
		assert(child);
		m_children.push_back(child);

		// Notify attributes and behaviours that a child has been attached to
		// this Game Object by immediately dispatching an event.
		m_eventDispatcher->Dispatch<Event::ChildGameObjectAttachedEvent>(child);
	}

	unsigned int GameObject::GetChildCount() const
	{
		return m_children.size();
	}

	std::shared_ptr<GameObject> GameObject::GetChild(unsigned int index) const
	{
		assert(index < m_children.size());
		return m_children[index];
	}

	void GameObject::Update(const double deltaTime)
	{
		// Update dispatcher.
		m_eventDispatcher->Update();

		// Update behaviours.
		if (!IsDead())
		{
			for (auto iter = m_behaviours.begin(); iter != m_behaviours.end(); ++iter)
			{
				std::shared_ptr<Behaviour::IBehaviour> behaviour = iter->second;
				behaviour->Update(deltaTime);
			}
		}

		// Update all living child Game Objects and remove those that have been
		// marked as dead.
		auto iter = m_children.begin();
		while (iter != m_children.end())
		{
			// Get the child Game Object.
			std::shared_ptr<GameObject> child = *iter;

			// Remove the child Game Object from the scene if it has been marked
			// as dead.
			if (child->IsDead())
			{
				iter = m_children.erase(iter);
			}
			else
			{
				child->Update(deltaTime);
				++iter;
			}
		}
	}

	std::shared_ptr<GameObject> GameObject::CreateGameObject()
	{
		std::shared_ptr<GameObject> childGameObject = std::make_shared<GameObject>(
			m_window,
			m_resourceManager,
			m_sceneEventDispatcher,
			shared_from_this()
		);

		AddChild(childGameObject);
		return childGameObject;
	}

	std::shared_ptr<GameObject> GameObject::CreateGameObject(const IGameObjectFactory& factory)
	{
		// Create a new Game Object.
		std::shared_ptr<GameObject> childGameObject = std::make_shared<GameObject>(
			m_window,
			m_resourceManager,
			m_sceneEventDispatcher,
			shared_from_this()
		);

		// Use the factory to decorate a new Game Object.
		factory.CreateGameObject(childGameObject);

		// Add the new Game Object as a child of this Game Object.
		AddChild(childGameObject);

		// Return shared pointer to the decorated child Game Object.
		return childGameObject;
	}
}
