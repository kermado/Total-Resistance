#ifndef EVENTDISPATCHER_H
#define	EVENTDISPATCHER_H

#include <memory>
#include <queue>
#include <map>
#include <typeinfo>
#include <cassert>
#include <functional>
#include <typeindex>

#include <Engine/NonCopyable.hpp>
#include <Engine/Event/IEvent.hpp>

/**
 * Creates callback function from the passed member function.
 *
 * @param memberFunction Member function.
 * @return Callback function.
 */
#define CALLBACK(memberFunction) \
std::bind(&memberFunction, this, std::placeholders::_1)

namespace Engine
{
	class EventDispatcher : private NonCopyable
	{
	public:
		/**
		 * Subscription identifier type definition.
		 */
		typedef unsigned int SubscriptionID;

		/**
		 * Constructor.
		 */
		EventDispatcher();

		/**
		 * Destructor.
		 */
		~EventDispatcher();

		/**
		 * Subscribe to receive events.
		 *
		 * @param callback Event handler callback.
		 * @return Subscription identifier for the subscription.
		 */
		template <typename EventType>
		SubscriptionID Subscribe(std::function<void(const EventType&)> callback)
		{
			std::shared_ptr<SpecificDispatcher<EventType>> dispatcher = GetSpecificDispatcher<EventType>();
			assert(dispatcher);
			dispatcher->Subscribe(m_nextSubscriptionId, callback);
			return m_nextSubscriptionId++; // NOTE: post-increment!
		}

		/**
		 * Unsubscribe to receive events.
		 *
		 * @param subscriptionId Subscription identifier to unsubscribe.
		 */
		template <typename EventType>
		void Unsubscribe(SubscriptionID subscriptionId)
		{
			std::shared_ptr<SpecificDispatcher<EventType>> dispatcher = GetSpecificDispatcher<EventType>();
			assert(dispatcher);
			dispatcher->Unsubscribe(subscriptionId);
		}

		/**
		 * Immediately dispatch an event to subscribers.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void Dispatch(const Arguments... args)
		{
			std::shared_ptr<SpecificDispatcher<EventType>> dispatcher = GetSpecificDispatcher<EventType>();
			assert(dispatcher);
			dispatcher->Dispatch(args...);
		}

		/**
		 * Queue an event to be dispatched to subscribers on the next Update
		 * call.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void Enqueue(const Arguments... args)
		{
			// Get the dispatcher for the event type.
			std::shared_ptr<SpecificDispatcher<EventType>> dispatcher = GetSpecificDispatcher<EventType>();
			assert(dispatcher);

			// Enqueue the event on the specific dispatcher.
			dispatcher->Enqueue(args...);

			// Record the event in the order list.
			m_eventOrder.emplace(typeid(EventType));
		}

		/**
		 * Dispatches all events in the queue to subscribers.
		 */
		void Update()
		{
			while (!m_eventOrder.empty())
			{
				// Get the type of the next event to process from the event
				// order list.
				const std::type_index eventType = m_eventOrder.front();
				m_eventOrder.pop();

				// Process the first event in the specific dispatcher for the
				// event type matching the determined next event.
				auto iter = m_dispatchers.find(eventType);
				assert(iter != m_dispatchers.end());
				std::shared_ptr<ISpecificDispatcher> dispatcher = iter->second;
				assert(dispatcher);
				dispatcher->ProcessSingleEvent();
			}
		}

	protected:
		/**
		 * Event specific dispatcher interface.
		 */
		class ISpecificDispatcher : private NonCopyable
		{
		public:
			virtual ~ISpecificDispatcher() {}
			virtual void ProcessSingleEvent() = 0;
		};

		/**
		 * Event specific dispatcher.
		 */
		template<typename EventType>
		class SpecificDispatcher : public ISpecificDispatcher
		{
		public:
			/**
			 * Constructor.
			 */
			SpecificDispatcher()
			: m_eventQueue()
			, m_callbacks()
			{
				// Nothing to do.
			}

			/**
			 * Destructor.
			 */
			~SpecificDispatcher()
			{
				// Nothing to do.
			}

			/**
			 * Subscribe to receive events.
			 *
			 * @param Subscription identifier for the subscription.
			 * @param callback Event handler callback.
			 */
			void Subscribe(SubscriptionID subscriptionId, std::function<void(const EventType&)> callback)
			{
				assert(m_callbacks.find(subscriptionId) == m_callbacks.end());
				m_callbacks[subscriptionId] = callback;
			}

			/**
			 * Unsubscribe to receive events.
			 *
			 * @param subscriptionId Subscription identifier to unsubscribe.
			 */
			void Unsubscribe(SubscriptionID subscriptionId)
			{
				if (subscriptionId > 0)
				{
					auto iter = m_callbacks.find(subscriptionId);
					if (iter != m_callbacks.end())
					{
						m_callbacks.erase(iter);
					}
				}
			}

			/**
			 * Immediately dispatch an event to subscribers.
			 *
			 * @param args... Event constructor arguments.
			 */
			template <typename... Arguments>
			void Dispatch(const Arguments... args)
			{
				EventType event(args...);
				for (auto iter = m_callbacks.begin(); iter != m_callbacks.end(); ++iter)
				{
					std::function<void(EventType& event)> callback = iter->second;
					assert(callback);
					callback(event);
				}
			}

			/**
			 * Queue an event to be broadcast to subscribers on the next Update
			 * call.
			 *
			 * @param args... Event constructor arguments.
			 */
			template <typename... Arguments>
			void Enqueue(const Arguments... args)
			{
				m_eventQueue.push(std::make_shared<EventType>(args...));
			}

			/**
			 * Notifies subscribers that are subscribed to receive the top event
			 * queued.
			 */
			virtual void ProcessSingleEvent()
			{
				if (!m_eventQueue.empty())
				{
					// Retrieve and remove the first event in the queue.
					std::shared_ptr<EventType> event = m_eventQueue.front();
					m_eventQueue.pop();

					// Publish the event to all subscribers.
					for (auto iter = m_callbacks.begin(); iter != m_callbacks.end(); ++iter)
					{
						std::function<void(const EventType& event)> callback = iter->second;
						callback(*event);
					}
				}
			}

		private:
			/**
			 * Queued events.
			 */
			std::queue<std::shared_ptr<EventType>> m_eventQueue;

			/**
			 * Event callback handlers.
			 */
			std::map<SubscriptionID, std::function<void(const EventType&)>> m_callbacks;
		};

		/**
		 * Returns a shared pointer to the specific dispatcher for the event
		 * type. A specific dispatcher is first created if one does not exist.
		 *
		 * @return Shared pointer to the dispatcher for the specified event
		 * type.
		 */
		template<typename EventType>
		std::shared_ptr<SpecificDispatcher<EventType>> GetSpecificDispatcher()
		{
			auto iter = m_dispatchers.find(std::type_index(typeid(EventType)));
			if (iter == m_dispatchers.end())
			{
				// A specific dispatcher was not found.
				// We better create one!
				std::shared_ptr<SpecificDispatcher<EventType>> specificDispatcher =
					std::make_shared<SpecificDispatcher<EventType>>();
				m_dispatchers[std::type_index(typeid(EventType))] = specificDispatcher;
				return specificDispatcher;
			}
			else
			{
				std::shared_ptr<ISpecificDispatcher> dispatcher = iter->second;
				assert(dispatcher);
				return std::static_pointer_cast<SpecificDispatcher<EventType>>(dispatcher);
			}
		}

	private:
		/**
		 * Next subscription identifier to use.
		 */
		SubscriptionID m_nextSubscriptionId;

		/**
		 * Event specific dispatchers.
		 */
		std::map<std::type_index, std::shared_ptr<ISpecificDispatcher>> m_dispatchers;

		/**
		 * Stores the order in which the events were enqueued.
		 */
		std::queue<std::type_index> m_eventOrder;
	};
}

#endif
