#include "Behaviour/ToggleRange.hpp"
#include "Event/ToggleRangeEvent.hpp"

namespace Behaviour
{
	ToggleRange::ToggleRange(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Model> modelAttribute)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_modelAttribute(modelAttribute)
	, m_toggleRangeSubscription(0)
	{
		// Subscribe for ToggleRangeEvents.
		m_toggleRangeSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::ToggleRangeEvent>(
			[this] (const Event::ToggleRangeEvent& event)
			{
				m_modelAttribute->SetVisible(event.GetVisible());
			}
		);
	}

	ToggleRange::~ToggleRange()
	{
		// Unsubscribe for ToggleRanageEvents.
		GetGameObjectEventDispatcher()->Unsubscribe<Event::ToggleRangeEvent>(m_toggleRangeSubscription);
	}

	void ToggleRange::Update(double deltaTime)
	{
		// Nothing to do.
	}
}
