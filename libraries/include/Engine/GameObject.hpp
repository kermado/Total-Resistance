#ifndef GAMEOBJECT_H
#define	GAMEOBJECT_H

#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <cassert>

#include <Engine/NonCopyable.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>
#include <Engine/IGameObjectFactory.hpp>

#include <Engine/Event/CreateGameObjectEvent.hpp>
#include <Engine/Event/DestroyGameObjectEvent.hpp>
#include <Engine/Attribute/IAttribute.hpp>
#include <Engine/Behaviour/IBehaviour.hpp>

namespace Engine
{
	class IGameScene;

	/**
	 * Represents a Game Object / Entity in the scene.
	 *
	 * @note Game Objects are not copyable. This is because copying a Game
	 * Object is undesirable most of the time. We enforce this so that the
	 * compiler will warn you if you unintentionally attempt to create
	 * a copy.
	 */
	class GameObject : private NonCopyable, public std::enable_shared_from_this<GameObject>
	{
	public:
		/**
		 * Game Object ID type.
		 */
		typedef unsigned int ID;

		/**
		 * Constructor.
		 *
		 * @param window Shared pointer to the window.
		 * @param resourceManager Shared pointer to the resource manager.
		 * @param sceneEventDispatcher Shared pointer to the scene's Event
		 * Dispatcher.
		 * @param gameScene Weak pointer to the game scene.
		 */
		GameObject(std::shared_ptr<Window> window,
			std::shared_ptr<ResourceManager> resourceManager,
			std::shared_ptr<EventDispatcher> sceneEventDispatcher);

		/**
		 * Constructor.
		 *
		 * @param window Shared pointer to the window.
		 * @param resourceManager Shared pointer to the resource manager.
		 * @param sceneEventDispatcher Shared pointer to the scene's Event
		 * Dispatcher.
		 * @param gameScene Weak pointer to the game scene.
		 * @param parent Shared pointer to the parent Game Object.
		 */
		GameObject(std::shared_ptr<Window> window,
			std::shared_ptr<ResourceManager> resourceManager,
			std::shared_ptr<EventDispatcher> sceneEventDispatcher,
			std::weak_ptr<GameObject> parent);

		/**
		 * Destructor.
		 */
		virtual ~GameObject();

		/**
		 * Returns the Game Object's unique identifier.
		 *
		 * @return Game Object's unique identifier.
		 */
		ID GetId() const;

		/**
		 * Returns true if the Game Object has been marked for removal.
		 *
		 * @return True if Game Object has been marked for removal.
		 */
		bool IsDead() const;

		/**
		 * Adds the specified Game Object as a child of this Game Object.
		 *
		 * @param child Shared pointer to the child Game Object.
		 */
		void AddChild(std::shared_ptr<GameObject> child);

		/**
		 * Returns the number of child Game Objects.
		 *
		 * @return Number of child Game Objects.
		 */
		unsigned int GetChildCount() const;

		/**
		 * Returns a shared pointer to the child Game Object specified by the
		 * provided index.
		 *
		 * @return Shared pointer to the child Game Object specified by the
		 * provided index.
		 */
		std::shared_ptr<GameObject> GetChild(unsigned int index) const;

		/**
		 * Enqueues an asynchronous event to the Game Object's Event Dispatcher.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void EnqueueEvent(Arguments... args)
		{
			m_eventDispatcher->Enqueue<EventType>(args...);
		}

		/**
		 * Dispatches an synchronous event to the Game Object's Event
		 * Dispatcher.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void DispatchEvent(Arguments... args)
		{
			m_eventDispatcher->Dispatch<EventType>(args...);
		}

		/**
		 * Creates and recursively enqueues events in the Game Object and all
		 * children down the Game Object tree.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void BroadcastEnqueue(const Arguments... args)
		{
			EnqueueEvent<EventType>(args...);
			DescendantBroadcastEnqueue<EventType>(args...);
		}

		/**
		 * Creates and recursively dispatches events in the Game Object and all
		 * children down the Game Object tree.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void BroadcastDispatch(const Arguments... args)
		{
			DispatchEvent<EventType>(args...);
			DescendantBroadcastDispatch<EventType>(args...);
		}

		/**
		 * Creates and recursively enqueues events down the Game Object tree.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void DescendantBroadcastEnqueue(const Arguments... args)
		{
			for (unsigned int i = 0; i < GetChildCount(); ++i)
			{
				GetChild(i)->BroadcastEnqueue<EventType>(args...);
			}
		}

		/**
		 * Creates and recursively dispatches events down the Game Object tree.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void DescendantBroadcastDispatch(const Arguments... args)
		{
			for (unsigned int i = 0; i < GetChildCount(); ++i)
			{
				GetChild(i)->BroadcastDispatch<EventType>(args...);
			}
		}

		/**
		 * Updates the Game Object by updating all of the attached behaviours.
		 *
		 * @param deltaTime Time since last call (in seconds).
		 */
		void Update(const double deltaTime);

		/**
		 * Creates and adds an attribute with the provided type to the Game
		 * Object. A shared pointer to the created attribute is returned.
		 *
		 * @param args... Attribute constructor arguments.
		 * @return Shared pointer to the created attribute.
		 */
		template<typename AttributeType, typename... Arguments>
		std::shared_ptr<AttributeType> CreateAttribute(Arguments... args)
		{
			std::shared_ptr<AttributeType> attribute = std::make_shared<AttributeType>(
				m_window,
				m_resourceManager,
				m_sceneEventDispatcher,
				m_eventDispatcher,
				shared_from_this(),
				args...
			);

			m_attributes[std::type_index(typeid(AttributeType))] = attribute;
			return attribute;
		}

		/**
		 * Returns true if the Game Object has an attribute with the provided
		 * type.
		 *
		 * @return True if the Game Object has an attribute matching the
		 * provided type.
		 */
		template<typename AttributeType>
		bool HasAttribute() const
		{
			return m_attributes.find(std::type_index(typeid(AttributeType))) != m_attributes.end();
		}

		/**
		 * Returns a pointer to the attribute with the provided type.
		 *
		 * @note Existence is not checked.
		 *
		 * @return Pointer to the attribute.
		 */
		template<typename AttributeType>
		std::shared_ptr<AttributeType> GetAttribute()
		{
			assert(m_attributes.find(std::type_index(typeid(AttributeType))) != m_attributes.end());
			std::shared_ptr<Attribute::IAttribute> attribute = m_attributes[std::type_index(typeid(AttributeType))];
			return std::static_pointer_cast<AttributeType>(attribute);
		}

		/**
		 * Creates and adds a behaviour with the provided type to the Game
		 * Object. A pointer to the created behaviour is returned.
		 *
		 * @param args... Behaviour constructor arguments.
		 * @return Pointer to the created behaviour.
		 */
		template<typename BehaviourType, typename... Arguments>
		std::shared_ptr<BehaviourType> CreateBehaviour(Arguments... args)
		{
			std::shared_ptr<BehaviourType> behaviour = std::make_shared<BehaviourType>(
				m_window,
				m_resourceManager,
				m_sceneEventDispatcher,
				m_eventDispatcher,
				shared_from_this(),
				args...
			);

			m_behaviours[std::type_index(typeid(BehaviourType))] = behaviour;
			return behaviour;
		}

		/**
		 * Creates a child game object and returns a shared pointer to the
		 * created child.
		 *
		 * @return Shared pointer to the created child game object.
		 */
		std::shared_ptr<GameObject> CreateGameObject();

		/**
		 * Creates a child Game Object from the specified Game Object factory
		 * instance.
		 *
		 * @param factory Reference to the factory to use.
		 * @return Shared pointer to the created Game Object.
		 */
		std::shared_ptr<GameObject> CreateGameObject(const IGameObjectFactory& factory);

	private:
		/**
		 * Shared pointer to the window.
		 */
		std::shared_ptr<Window> m_window;

		/**
		 * Shared pointer to the resource manager.
		 */
		std::shared_ptr<ResourceManager> m_resourceManager;

		/**
		 * Shared pointer to the event dispatcher for the scene that the Game
		 * Object belongs to.
		 */
		std::shared_ptr<EventDispatcher> m_sceneEventDispatcher;

		/**
		 * Event Dispatcher.
		 */
		std::shared_ptr<EventDispatcher> m_eventDispatcher;

		/**
		 * Identifier for the Game Object.
		 */
		ID m_identifier;

		/**
		 * Should the Game Object be removed from the scene at the next update?
		 */
		bool m_dead;

		/**
		 * Child Game Objects.
		 */
		std::vector<std::shared_ptr<GameObject>> m_children;

		/**
		 * Attributes belonging to the Game Object.
		 */
		std::unordered_map<std::type_index, std::shared_ptr<Attribute::IAttribute>> m_attributes;

		/**
		 * Behaviours belonging to the Game Object.
		 */
		std::unordered_map<std::type_index, std::shared_ptr<Behaviour::IBehaviour>> m_behaviours;

		/**
		 * Subscription identifier for the CreateGameObjectEvent subscription.
		 */
		EventDispatcher::SubscriptionID m_createGameObjectSubscription;

		/**
		 * Subscription identifier for the DestroyGameObjectEvent subscription.
		 */
		EventDispatcher::SubscriptionID m_destroyGameObjectSubscription;

	private:
		/**
		 * Next identifier to use.
		 */
		static ID s_nextIdentifier;
	};
}

#endif
