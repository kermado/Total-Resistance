#ifndef THREADEVENTRECEIVER_H
#define	THREADEVENTRECEIVER_H

#include <mutex>
#include <memory>
#include <queue>
#include <deque>
#include <typeindex>
#include <map>
#include <cassert>

#include <Engine/NonCopyable.hpp>
#include <Engine/Event/IEvent.hpp>

#ifndef CALLBACK
	/**
	 * Helper macro creates a member function callback.
	 */
	#define CALLBACK(memberFunction) \
	std::bind(&memberFunction, this, std::placeholders::_1)
#endif

namespace Engine
{
	namespace Event
	{
		/**
		 * Stop thread event.
		 */
		class StopThreadEvent : private IEvent
		{
		public:
			StopThreadEvent()
			: IEvent()
			{
				// Nothing to do.
			}

			virtual ~StopThreadEvent()
			{
				// Nothing to do.
			}
		};
	}

	/**
	 * Thread-safe event queue.
	 */
	template <typename EventType>
	class ThreadSafeEventQueue : private NonCopyable
	{
	public:
		/**
		 * Constructor.
		 */
		ThreadSafeEventQueue()
		: m_queueMutex()
		, m_queue()
		{
			// Nothing to do.
		}

		/**
		 * Destructor.
		 */
		~ThreadSafeEventQueue()
		{
			// Nothing to do.
		}

		/**
		 * Pushes the supplied event onto the queue.
		 *
		 * @param message Event to push onto the queue.
		 */
		void Push(std::shared_ptr<EventType> event)
		{
			std::lock_guard<std::mutex> lock(m_queueMutex);

			// Wrap the posted event in a WrappedEvent
			// and store a pointer to this WrappedMessage in the queue.
			m_queue.push(event);
		}

		/**
		 * Pops the top event off the queue and returns a shared
		 * pointer to that event.
		 *
		 * @return Shared pointer to the popped message.
		 */
		std::shared_ptr<EventType> Pop()
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);

			if (!m_queue.empty())
			{
				// Pop and return a shared pointer to the top message
				// on the queue.
				std::shared_ptr<EventType> top = m_queue.front();
				m_queue.pop();
				return top;
			}
			else
			{
				// Can't pop from an empty queue.
				assert(0);
				return nullptr; // Stop the compiler from complaining.
			}
		}

		/**
		 * Returns true if the queue is empty.
		 *
		 * @return True if the queue is empty.
		 */
		bool Empty()
		{
			// Note: Mutex is unlocked in the destructor
			// of the std::lock_guard.
			std::lock_guard<std::mutex> lock(m_queueMutex);
			return m_queue.empty();
		}

	private:
		/**
		 * Mutex to guard access to the internal queue
		 * data structure.
		 */
		std::mutex m_queueMutex;

		/**
		 * The actual queue.
		 * This stores pointers to BaseMessage instances.
		 */
		std::queue<std::shared_ptr<EventType>> m_queue;
	};

	/**
	 * Thread-safe event dispatcher.
	 *
	 * Clients can enqueue events and subscribe to receive events of a specifed
	 * type. The @see Update method notifies subscribers by processing events in
	 * the order in which the events were enqueued.
	 */
	class ThreadEventReceiver : private NonCopyable
	{
	public:
		/**
		 * Subscription identifier type definition.
		 */
		typedef unsigned int SubscriptionID;

		/**
		 * Constructor.
		 */
		ThreadEventReceiver()
		: m_nextSubscriptionId(1)
		, m_receivers()
		, m_eventOrder()
		, m_eventOrderMutex()
		{
			// Nothing to do.
		}

		/**
		 * Destructor.
		 */
		~ThreadEventReceiver()
		{
			// Nothing to do.
		}

		/**
		 * Subscribes the callback function to receive events of the specified
		 * type.
		 *
		 * @param callback Callback function.
		 */
		template <typename EventType>
		SubscriptionID Subscribe(std::function<void(const EventType&)> callback)
		{
			std::shared_ptr<SpecificThreadEventReceiver<EventType>> receiver =
				GetSpecificReceiver<EventType>();
			assert(receiver);
			receiver->Subscribe(m_nextSubscriptionId, callback);
			return m_nextSubscriptionId++;
		}

		/**
		 * Unsubscribe to receive events.
		 *
		 * @param subscriptionId Subscription identifier to unsubscribe.
		 */
		template <typename EventType>
		void Unsubscribe(SubscriptionID subscriptionId)
		{
			std::shared_ptr<SpecificThreadEventReceiver<EventType>> receiver =
				GetSpecificReceiver<EventType>();
			assert(receiver);
			receiver->Unsubscribe(subscriptionId);
		}

		/**
		 * Enqueues an event of the specified type to be broadcast to
		 * subscribers.
		 *
		 * @param args... Event constructor arguments.
		 */
		template<typename EventType, typename... Arguments>
		void Enqueue(const Arguments... args)
		{
			// Enqueue the event with the relevant specific receiver.
			std::shared_ptr<SpecificThreadEventReceiver<EventType>> receiver =
				GetSpecificReceiver<EventType>();
			receiver->Enqueue(args...);

			// Note: Mutex is unlocked in the destructor
			// of the std::lock_guard.
			std::lock_guard<std::mutex> lock(m_eventOrderMutex);

			// Record the event in the order list.
			std::type_index eventType = typeid(EventType);
			if (eventType == typeid(Event::StopThreadEvent))
			{
				m_eventOrder.emplace_front(eventType);
			}
			else
			{
				m_eventOrder.emplace_back(eventType);
			}
		}

		/**
		 * Processes a single event by notifying subscribers that are subscribed
		 * to receive the top event currently queued.
		 */
		void ProcessSingleEvent()
		{
			if (!m_eventOrder.empty())
			{
				std::shared_ptr<ISpecificThreadEventReceiver> receiver = nullptr;

				// Scope for mutex releasing.
				{
					// Note: Mutex is unlocked in the destructor
					// of the std::lock_guard.
					std::lock_guard<std::mutex> lock(m_eventOrderMutex);

					// Get the type of the next event to process from the event
					// order list.
					const std::type_index eventType = m_eventOrder.front();
					m_eventOrder.pop_front();

					// Process the first event in the specific listener for the
					// event type matching the determined next event.
					auto iter = m_receivers.find(eventType);
					assert(iter != m_receivers.end());
					receiver = iter->second;
				}

				// Process the event.
				if (receiver) receiver->ProcessSingleEvent();
			}
		}

		/**
		 * Notifies subscribers that are subscribed to receive events currently
		 * queued.
		 */
		void Update()
		{
			while (!m_eventOrder.empty())
			{
				std::shared_ptr<ISpecificThreadEventReceiver> receiver = nullptr;

				// Scope for mutex releasing.
				{
					// Note: Mutex is unlocked in the destructor
					// of the std::lock_guard.
					std::lock_guard<std::mutex> lock(m_eventOrderMutex);

					// Get the type of the next event to process from the event
					// order list.
					const std::type_index eventType = m_eventOrder.front();
					m_eventOrder.pop_front();

					// Process the first event in the specific listener for the
					// event type matching the determined next event.
					auto iter = m_receivers.find(eventType);
					assert(iter != m_receivers.end());
					receiver = iter->second;
				}

				// Process the event.
				if (receiver) receiver->ProcessSingleEvent();
			}
		}

	protected:
		/**
		 * Event specific receiver interface.
		 */
		class ISpecificThreadEventReceiver : private NonCopyable
		{
		public:
			/**
			 * Destructor.
			 */
			virtual ~ISpecificThreadEventReceiver();

			/**
			 * Processes a single event by notifying subscribers of the event.
			 */
			virtual void ProcessSingleEvent() = 0;
		};

		/**
		 * Event specific receiver.
		 */
		template <typename EventType>
		class SpecificThreadEventReceiver : public ISpecificThreadEventReceiver
		{
		public:
			/**
			 * Callback function type.
			 */
			typedef std::function<void(const EventType&)> Callback;

			/**
			 * Constructor.
			 */
			SpecificThreadEventReceiver()
			: m_eventQueue()
			, m_subscribers()
			{
				// Nothing to do.
			}

			/**
			 * Destructor.
			 */
			~SpecificThreadEventReceiver()
			{
				// Nothing to do.
			}

			/**
			 * Subscribe to receive events.
			 *
			 * @param Subscription identifier for the subscription.
			 * @param callback Event handler callback.
			 */
			void Subscribe(SubscriptionID subscriptionId, Callback subscriber)
			{
				assert(m_subscribers.find(subscriptionId) == m_subscribers.end());
				m_subscribers[subscriptionId] = subscriber;
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
					auto iter = m_subscribers.find(subscriptionId);
					if (iter != m_subscribers.end())
					{
						m_subscribers.erase(iter);
					}
				}
			}

			/**
			 * Queues an event to be broadcast to subscribers.
			 *
			 * @param args... Event constructor arguments.
			 */
			template<typename... Arguments>
			void Enqueue(const Arguments... args)
			{
				m_eventQueue.Push(std::make_shared<EventType>(args...));
			}

			/**
			 * Notifies subscribers that are subscribed to receive
			 * the top event queued.
			 */
			virtual void ProcessSingleEvent()
			{
				if (!m_eventQueue.Empty())
				{
					// Pop and return the top event on the queue.
					std::shared_ptr<EventType> event = m_eventQueue.Pop();

					// Publish the event to all subscribers.
					for (auto iter = m_subscribers.begin(); iter != m_subscribers.end(); ++iter)
					{
						Callback callback = iter->second;
						callback(*event);
					}
				}
			}

		private:
			/**
			 * The thread safe event queue for the receiver.
			 */
			ThreadSafeEventQueue<EventType> m_eventQueue;

			/**
			 * Subscriber callback functions.
			 */
			std::map<SubscriptionID, Callback> m_subscribers;
		};

		/**
		 * Returns a pointer to the specific event receiver for the specified
		 * event type.
		 *
		 * @return Pointer to the receiver for the specified event
		 * type.
		 */
		template<typename EventType>
		std::shared_ptr<SpecificThreadEventReceiver<EventType>> GetSpecificReceiver()
		{
			auto iter = m_receivers.find(std::type_index(typeid(EventType)));
			if (iter == m_receivers.end())
			{
				// A specific receicer was not found. We better create one!
				std::shared_ptr<SpecificThreadEventReceiver<EventType>> specificReceiver =
					std::make_shared<SpecificThreadEventReceiver<EventType>>();
				m_receivers[std::type_index(typeid(EventType))] =
					std::static_pointer_cast<ISpecificThreadEventReceiver>(specificReceiver);

				// Return a shared pointer to the created specific receiver.
				return specificReceiver;
			}
			else
			{
				// Return the existing receiver.
				return std::static_pointer_cast<SpecificThreadEventReceiver<EventType>>(iter->second);
			}
		}

	private:
		/**
		 * Next subscription identifier to use.
		 */
		SubscriptionID m_nextSubscriptionId;

		/**
		 * Specific event type receivers.
		 */
		std::map<std::type_index, std::shared_ptr<ISpecificThreadEventReceiver>> m_receivers;

		/**
		 * Stores the order in which the events were enqueued.
		 */
		std::deque<std::type_index> m_eventOrder;

		/**
		 * Mutex to guard access to event order data structure.
		 */
		std::mutex m_eventOrderMutex;
	};
}

#endif
