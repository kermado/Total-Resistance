#include "Attribute/Health.hpp"

#include <Engine/Audio.hpp>

#include <Engine/Event/DestroyGameObjectEvent.hpp>

#include "ExplosionFactory.hpp"
#include "WallFactory.hpp"

#include "Event/EnemyDestroyedEvent.hpp"

namespace Attribute
{
	Health::Health(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		float initialHealth)
	: IAttribute(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_health(initialHealth)
	, m_dead(false)
	, m_inflictDamageSubscription(0)
	{
		// Subscribe for InflictDamageEvents.
		m_inflictDamageSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::InflictDamageEvent>(CALLBACK(Health::HandleInflictDamageEvent));
	}

	Health::~Health()
	{
		// Unsubscribe for InflictDamageEvents.
		GetGameObjectEventDispatcher()->Unsubscribe<Event::InflictDamageEvent>(m_inflictDamageSubscription);
	}

	float Health::GetHealth() const
	{
		return m_health;
	}

	void Health::HandleInflictDamageEvent(const Event::InflictDamageEvent& event)
	{
		float damage = event.GetDamage();
		assert(damage > 0.0f);

		// Inflict the requested damage.
		m_health -= damage;

		// Request to destroy the Game Object if there is no health remaining.
		// Publish an event to the scene in order to notify others that an enemy
		// was destroyed.
		if (!m_dead && m_health <= 0.0f)
		{
			m_dead = true;
			GetSceneEventDispatcher()->Enqueue<Event::EnemyDestroyedEvent>(GetGameObject().lock());
			GetGameObjectEventDispatcher()->Enqueue<Engine::Event::DestroyGameObjectEvent>();
			Engine::Audio::GetInstance().Play(GetResourceManager()->GetAudio("resources/audio/SmallExplosion.wav"));
		}
	}
}
