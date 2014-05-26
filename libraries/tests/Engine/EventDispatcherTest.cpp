#define BOOST_TEST_MODULE EventDispatcherTest
#include <boost/test/included/unit_test.hpp>
#include <Engine/EventDispatcher.hpp>
#include <Engine/Event/IEvent.hpp>

/**
 * Base event for testing.
 */
class BaseTestEvent : public Engine::Event::IEvent
{
public:
	/**
	 * Constructor.
	 *
	 * @param val Value to be carried by the event.
	 */
	BaseTestEvent(const int val):
	Engine::Event::IEvent()
	{
		value = val;
	}

	/**
	 * Returns the value carried by the event.
	 *
	 * @return Event payload.
	 */
	int GetValue() const
	{
		return value;
	}

private:
	/**
	 * Payload.
	 */
	int value;
};

/**
 * Simple event that carries a single integer value.
 */
class TestEvent : public BaseTestEvent
{
public:
	/**
	 * Constructor.
	 *
	 * @param val Value to be carried by the event.
	 */
	TestEvent(const int val) :
	BaseTestEvent(val)
	{
		// Nothing to do.
	}
};

/**
 * Another simple event that carries a single integer value.
 */
class OtherTestEvent : public BaseTestEvent
{
public:
	/**
	 * Constructor.
	 *
	 * @param val Value to be carried by the event.
	 */
	OtherTestEvent(const int val) :
	BaseTestEvent(val)
	{
		// Nothing to do.
	}
};

/**
 * Event sender.
 *
 * Dispatches and queues events on the passed Event Dispatcher instance.
 */
class TestEventSender
{
public:
	/**
	 * Constructor.
	 *
	 * @param dsptchr Event Dispatcher instance for sending events.
	 */
	TestEventSender(Engine::EventDispatcher& dsptchr) :
	dispatcher(dsptchr)
	{
		// Nothing to do.
	}

	/**
	 * Dispatches a TestEvent with the specified value.
	 *
	 * @param value Value for the TestEvent to dispatch.
	 */
	template<typename EventType>
	void Dispatch(const int value)
	{
		dispatcher.Dispatch<EventType>(value);
	}

	/**
	 * Enqueues a TestEvent with the specified value.
	 *
	 * @param value Value for the TestEvent to enqueue.
	 */
	template<typename EventType>
	void Enqueue(const int value)
	{
		dispatcher.Enqueue<EventType>(value);
	}

private:
	/**
	 * Event Dispatcher to use for sending events.
	 */
	Engine::EventDispatcher& dispatcher;
};

/**
 * Event receiver.
 *
 * Subscribes to receive TestEvents from the passed Event Dispatcher instance.
 * The last TestEvent payload is stored so that it can be compared with the
 * value originally sent.
 */
template<typename EventType>
class TestEventReceiver
{
public:
	/**
	 * Constructor.
	 *
	 * @param dsptchr Event Dispatcher instance on which to subscribe to receive
	 * TestEvents.
	 */
	TestEventReceiver(Engine::EventDispatcher& dsptchr) :
	valueReceived(0)
	{
		dsptchr.Subscribe<EventType>(CALLBACK(TestEventReceiver::Handler));
	}

	/**
	 * TestEvent callback handler.
	 *
	 * @param event Reference to the TestEvent.
	 */
	void Handler(const EventType& event)
	{
		valueReceived = event.GetValue();
	}

	/**
	 * Returns the value stored in the last TestEvent received by the Handler.
	 *
	 * @return Payload value received by the last event.
	 */
	int GetValueReceived()
	{
		return valueReceived;
	}

private:
	/**
	 * Payload value received by the last event
	 */
	int valueReceived;
};

/**
 * Test fixture.
 *
 * Sets up the environment before a test case is executed and cleans up after
 * each test case is complete.
 */
struct EventDispatcherTestFixture {
	/**
	 * Setup.
	 */
	EventDispatcherTestFixture() {}

	/**
	 * Teardown.
	 */
	~EventDispatcherTestFixture() {}

	/**
	 * Dispatcher.
	 */
	Engine::EventDispatcher dispatcher;
};

/**
 * Ensure that a single event handler receives the dispatched event to which it
 * is subscribed.
 */
BOOST_FIXTURE_TEST_CASE(TestSingleHandlerReceivesDispatchedEvent1,
	EventDispatcherTestFixture)
{
	// Create the event reciever.
	TestEventReceiver<TestEvent> receiver(dispatcher);

	// Create the event sender.
	TestEventSender sender(dispatcher);

	// Value to carry in event.
	int valueToSend = 0;

	// Dispatch a TestEvent, carrying the value.
	sender.Dispatch<TestEvent>(valueToSend);

	// Get the value received by the receiver.
	int valueReceived = receiver.GetValueReceived();

	// The sent and received value should be equal.
	BOOST_CHECK_EQUAL(valueToSend, valueReceived);
}

/**
 * Ensure that a single event handler receives the dispatched event to which it
 * is subscribed.
 */
BOOST_FIXTURE_TEST_CASE(TestSingleHandlerReceivesDispatchedEvent2,
	EventDispatcherTestFixture)
{
	// Create the event reciever.
	TestEventReceiver<TestEvent> receiver(dispatcher);

	// Create the event sender.
	TestEventSender sender(dispatcher);

	// Value to carry in event.
	int valueToSend = 1;

	// Dispatch a TestEvent, carrying the value.
	sender.Dispatch<TestEvent>(valueToSend);

	// Get the value received by the receiver.
	int valueReceived = receiver.GetValueReceived();

	// The sent and received value should be equal.
	BOOST_CHECK_EQUAL(valueToSend, valueReceived);
}

/**
 * Ensure that multiple event handler receive the dispatched event to which they
 * are both subscribed.
 */
BOOST_FIXTURE_TEST_CASE(TestMultipleHandlersReceiveDispatchedEvent,
	EventDispatcherTestFixture)
{
	// Create two event recievers.
	TestEventReceiver<TestEvent> receiver1(dispatcher);
	TestEventReceiver<TestEvent> receiver2(dispatcher);

	// Create the event sender.
	TestEventSender sender(dispatcher);

	// Value to carry in event.
	int valueToSend = 3;

	// Dispatch a TestEvent, carrying the value.
	sender.Dispatch<TestEvent>(valueToSend);

	// Get the values received by the two receivers.
	int valueReceivedByReceiver1 = receiver1.GetValueReceived();
	int valueReceivedByReceiver2 = receiver2.GetValueReceived();

	// The sent and received value should be equal for both receivers.
	BOOST_CHECK_EQUAL(valueToSend, valueReceivedByReceiver1);
	BOOST_CHECK_EQUAL(valueToSend, valueReceivedByReceiver2);
}

/**
 * Ensure that a single event handler does not receive the enqueued event to
 * which it is subscribed before the event queue has been processed.
 */
BOOST_FIXTURE_TEST_CASE(
	TestSingleHandlerDoesNotReceiveEnqueuedEventBeforeUpdate,
	EventDispatcherTestFixture)
{
	// Create the event reciever.
	TestEventReceiver<TestEvent> receiver(dispatcher);

	// Create the event sender.
	TestEventSender sender(dispatcher);

	// Value to carry in event.
	int valueToSend = 4;

	// Enqueue a TestEvent, carrying the value.
	sender.Enqueue<TestEvent>(valueToSend);

	// Get the value received by the receiver.
	int valueReceived = receiver.GetValueReceived();

	// The received value should be zero - indicating that no value has been
	// received yet.
	BOOST_CHECK_EQUAL(0, valueReceived);
}

/**
 * Ensure that a single event handler receives the enqueued event to which it
 * is subscribed.
 */
BOOST_FIXTURE_TEST_CASE(TestSingleHandlerReceivesEnqueuedEvent1,
	EventDispatcherTestFixture)
{
	// Create the event reciever.
	TestEventReceiver<TestEvent> receiver(dispatcher);

	// Create the event sender.
	TestEventSender sender(dispatcher);

	// Value to carry in event.
	int valueToSend = 5;

	// Enqueue a TestEvent, carrying the value.
	sender.Enqueue<TestEvent>(valueToSend);

	// Process the event queue.
	dispatcher.Update();

	// Get the value received by the receiver.
	int valueReceived = receiver.GetValueReceived();

	// The sent and received value should be equal.
	BOOST_CHECK_EQUAL(valueToSend, valueReceived);
}

/**
 * Ensure that multiple event handlers receive the enqueued event to which they
 * are both subscribed.
 */
BOOST_FIXTURE_TEST_CASE(TestMultipleHandlersReceiveEnqueuedEvent,
	EventDispatcherTestFixture)
{
	// Create two event recievers.
	TestEventReceiver<TestEvent> receiver1(dispatcher);
	TestEventReceiver<TestEvent> receiver2(dispatcher);

	// Create the event sender.
	TestEventSender sender(dispatcher);

	// Value to carry in event.
	int valueToSend = 6;

	// Enqueue a TestEvent, carrying the value.
	sender.Enqueue<TestEvent>(valueToSend);

	// Process the event queue.
	dispatcher.Update();

	// Get the values received by the two receivers.
	int valueReceivedByReceiver1 = receiver1.GetValueReceived();
	int valueReceivedByReceiver2 = receiver2.GetValueReceived();

	// The sent and received value should be equal for both receivers.
	BOOST_CHECK_EQUAL(valueToSend, valueReceivedByReceiver1);
	BOOST_CHECK_EQUAL(valueToSend, valueReceivedByReceiver2);
}

/**
 * Ensure that multiple event handlers each receive the dispatched events to
 * which they are both subscribed.
 */
BOOST_FIXTURE_TEST_CASE(TestMultipleHandlersReceiveDifferentDispatchedEvents,
	EventDispatcherTestFixture)
{
	// Create two event recievers.
	TestEventReceiver<TestEvent> receiver1(dispatcher);
	TestEventReceiver<OtherTestEvent> receiver2(dispatcher);

	// Create the event sender.
	TestEventSender sender(dispatcher);

	// Values to carry in events.
	int valueToSend1 = 7;
	int valueToSend2 = 8;

	// Dispatch a TestEvent, carrying the first value.
	sender.Dispatch<TestEvent>(valueToSend1);

	// Dispatch a OtherTestEvent, carrying the second value.
	sender.Dispatch<OtherTestEvent>(valueToSend2);

	// Get the values received by the two receivers.
	int valueReceivedByReceiver1 = receiver1.GetValueReceived();
	int valueReceivedByReceiver2 = receiver2.GetValueReceived();

	// Match the sent and received values.
	BOOST_CHECK_EQUAL(valueToSend1, valueReceivedByReceiver1);
	BOOST_CHECK_EQUAL(valueToSend2, valueReceivedByReceiver2);
}

/**
 * Ensure that multiple event handlers each receive the enqueued events to
 * which they are both subscribed.
 */
BOOST_FIXTURE_TEST_CASE(TestMultipleHandlersReceiveDifferentEnqueuedEvents,
	EventDispatcherTestFixture)
{
	// Create two event recievers.
	TestEventReceiver<TestEvent> receiver1(dispatcher);
	TestEventReceiver<OtherTestEvent> receiver2(dispatcher);

	// Create the event sender.
	TestEventSender sender(dispatcher);

	// Values to carry in events.
	int valueToSend1 = 7;
	int valueToSend2 = 8;

	// Enqueue a TestEvent, carrying the first value.
	sender.Enqueue<TestEvent>(valueToSend1);

	// Enqueue a OtherTestEvent, carrying the second value.
	sender.Enqueue<OtherTestEvent>(valueToSend2);

	// Process the event queue.
	dispatcher.Update();

	// Get the values received by the two receivers.
	int valueReceivedByReceiver1 = receiver1.GetValueReceived();
	int valueReceivedByReceiver2 = receiver2.GetValueReceived();

	// Match the sent and received values.
	BOOST_CHECK_EQUAL(valueToSend1, valueReceivedByReceiver1);
	BOOST_CHECK_EQUAL(valueToSend2, valueReceivedByReceiver2);
}

/**
 * Ensure that the event queue is emptied after it has been processed.
 */
BOOST_FIXTURE_TEST_CASE(TestEventQueueEmptiedAfterUpdate,
	EventDispatcherTestFixture)
{
	// Create the event sender.
	TestEventSender sender(dispatcher);

	// Value to carry in event.
	int valueToSend = 9;

	// Enqueue a TestEvent, carrying the first value.
	sender.Enqueue<TestEvent>(valueToSend);

	// Process the event queue.
	dispatcher.Update();

	// Create the event receiver.
	TestEventReceiver<TestEvent> receiver(dispatcher);

	// Process the event queue again.
	dispatcher.Update();

	// Get the value received by the receivers.
	int valueReceivedByReceiver = receiver.GetValueReceived();

	// Check that the value is zero - indicating that no value was received.
	BOOST_CHECK_EQUAL(0, valueReceivedByReceiver);
}
