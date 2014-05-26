#include "Round.hpp"

#include <algorithm>

#include <Engine/Event/CreateGameObjectEvent.hpp>

#include "Event/EnemyDestroyedEvent.hpp"
#include "Event/EnemySurvivedEvent.hpp"

Round::Round(std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
	double duration)
: m_sceneEventDispatcher(sceneEventDispatcher)
, m_duration(duration)
, m_elapsedTime(0.0)
, m_spawnSequence()
, m_unitCount(0)
, m_spawnCount(0)
, m_finishedCount(0)
, m_enemyDestroyedSubscription(0)
, m_enemySurvivedSubscription(0)
{
	// Subscribe for EnemyDestroyedEvents.
	m_enemyDestroyedSubscription = m_sceneEventDispatcher->Subscribe<Event::EnemyDestroyedEvent>(
		[this] (const Event::EnemyDestroyedEvent& event) {
			if (m_spawnCount > 0)
			{
				++m_finishedCount;
			}
		}
	);

	// Subscribe for EnemySurvivedEvents.
	m_enemySurvivedSubscription = m_sceneEventDispatcher->Subscribe<Event::EnemySurvivedEvent>(
		[this] (const Event::EnemySurvivedEvent& event) {
			if (m_spawnCount > 0)
			{
				++m_finishedCount;
			}
		}
	);
}

Round::~Round()
{
	// Unsubscribe for events.
	m_sceneEventDispatcher->Unsubscribe<Event::EnemyDestroyedEvent>(m_enemyDestroyedSubscription);
	m_sceneEventDispatcher->Unsubscribe<Event::EnemySurvivedEvent>(m_enemySurvivedSubscription);
}

void Round::Spawn(std::shared_ptr<const Engine::IGameObjectFactory> factory)
{
	m_spawnSequence.push_back(factory);
	++m_unitCount;
}

void Round::Shuffle()
{
	std::random_shuffle(m_spawnSequence.begin(), m_spawnSequence.end());
}

void Round::Update(double deltaTime)
{
	if (!m_spawnSequence.empty())
	{
		// Increment the timer.
		m_elapsedTime += deltaTime;

		// Calculate the time between unit spawns.
		const double interval = m_duration / static_cast<double>(m_unitCount);

		// Calculate the number of units that need to be spawned.
		int unitsToSpawn = (m_elapsedTime / interval) - m_spawnCount;

		// Request to spawn the units.
		for (int i = 0; i < unitsToSpawn; ++i)
		{
			std::shared_ptr<const Engine::IGameObjectFactory> factory = m_spawnSequence.front();
			if (!m_spawnSequence.empty()) m_spawnSequence.pop_front();
			m_sceneEventDispatcher->Enqueue<Engine::Event::CreateGameObjectEvent>(factory);

			// Increment the spawn counter.
			++m_spawnCount;
		}
	}
}

bool Round::IsFinished() const
{
	return m_spawnSequence.empty() && m_unitCount == m_finishedCount;
}
