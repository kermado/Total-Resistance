#ifndef ROUND_H
#define ROUND_H

#include <deque>
#include <memory>

#include <Engine/NonCopyable.hpp>
#include <Engine/EventDispatcher.hpp>
#include <Engine/IGameObjectFactory.hpp>

class Round : public Engine::NonCopyable
{
public:
	/**
	 * Constructor.
	 *
	 * @param sceneEventDispatcher Shared pointer to the scene's event
	 * dispatcher.
	 * @param duration Round duration (in seconds).
	 */
	Round(std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		double duration);

	/**
	 * Destructor.
	 */
	~Round();

	/**
	 * Records that a unit should be spawned from the specified factory.
	 *
	 * @note Units are spawned in the order requested.
	 *
	 * @param factory Shared pointer to the game object factory from which to
	 * construct the unit.
	 */
	void Spawn(std::shared_ptr<const Engine::IGameObjectFactory> factory);

	/**
	 * Shuffles the order in which the units will be spawned.
	 */
	void Shuffle();

	/**
	 * Runs the round.
	 *
	 * @param deltaTime Time since the last call (in seconds).
	 */
	void Update(double deltaTime);

	/**
	 * Returns true if the round has come to an end.
	 *
	 * @return True if the round has finished.
	 */
	bool IsFinished() const;

private:
	/**
	 * Shared pointer to the scene's event dispatcher.
	 */
	std::shared_ptr<Engine::EventDispatcher> m_sceneEventDispatcher;

	/**
	 * The round duration (in seconds).
	 */
	double m_duration;

	/**
	 * The amount of elapsed time (in seconds).
	 */
	double m_elapsedTime;

	/**
	 * The unit spawn sequence for the round.
	 */
	std::deque<std::shared_ptr<const Engine::IGameObjectFactory>> m_spawnSequence;

	/**
	 * The total number of units that were requested to be spawned over the
	 * duration of the round.
	 */
	unsigned int m_unitCount;

	/**
	 * The number of units spawned thus far.
	 */
	unsigned int m_spawnCount;

	/**
	 * The number of units that survived or have been destroyed.
	 */
	unsigned int m_finishedCount;

	/**
	 * Subscription identifier for the EnemyDestroyedEvent subscription.
	 */
	Engine::EventDispatcher::SubscriptionID m_enemyDestroyedSubscription;

	/**
	 * Subscription identifier for the EnemySurvivedEvent subscription.
	 */
	Engine::EventDispatcher::SubscriptionID m_enemySurvivedSubscription;
};

#endif
