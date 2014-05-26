#ifndef GAMESCENE_H
#define	GAMESCENE_H

#include <queue>

#include <Engine/IGameScene.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Plane.hpp>
#include <Engine/Event/MouseButtonPressedEvent.hpp>
#include <Engine/UI/Rectangle.hpp>
#include <Engine/UI/Font.hpp>
#include <Engine/UI/Text.hpp>

#include "Grid.hpp"
#include "PlayingSurface.hpp"
#include "Round.hpp"

class GameScene : public Engine::IGameScene
{
public:
	/**
	 * Constructor.
	 *
	 * @param window Shared pointer to the window.
	 * @param resourceManager Shared pointer to the resource manager.
	 * @param sceneStackEventDispatcher Shared pointer to the scene stack's
	 * event dispatcher.
	 */
	GameScene(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneStackEventDispatcher);

	/**
	 * Destructor.
	 */
	virtual ~GameScene();

	/**
	 * Scene was created.
	 */
	virtual void OnCreate();

	/**
	 * Scene was suspended.
	 */
	virtual void OnSuspend();

	/**
	 * Scene was resumed.
	 */
	virtual void OnResume();

	/**
	 * Updates the scene.
	 *
	 * @param deltaTime Time since last call (in seconds).
	 */
	virtual void OnUpdate(double deltaTime);

	/**
	 * Draws the user interface.
	 */
	virtual void OnDrawUI();

	/**
	 * Handles MouseButtonPressedEvents.
	 *
	 * @param event Reference to the event.
	 */
	void HandleMouseButtonPressedEvent(const Engine::Event::MouseButtonPressedEvent& event);

protected:
	/**
	 * Returns the round information string to be displayed by the round label.
	 *
	 * @return Round information string for the round label..
	 */
	std::string GetRoundsLabelString() const;

protected:
	/**
	 * Returns the playing grid cell corresponding to the specified screen
	 * point.
	 *
	 * @note The cell may be outside the range of the playing grid.
	 *
	 * @param screenPoint Screen point.
	 * @return Corresponding playing grid cell.
	 */
	Cell ScreenPointToCell(const glm::vec2& screenPoint) const;

	/**
	 * Returns the specified cell's centroid position in world coordinates.
	 *
	 * @param cell Cell to determine centroid position for.
	 * @return Centroid position for the specified cell.
	 */
	glm::vec3 GetCellCentroidPosition(Cell cell) const;

protected:
	/**
	 * (Re)positions the UI elements.
	 */
	void PositionUI();

	/**
	 * Creates a number of rounds.
	 */
	void SetupRounds();

	/**
	 * Subscribes the scene to receive various events.
	 */
	void SubscribeForEvents();

	/**
	 * Unsubscribes the scene to receive the events that were subscribed to in
	 * the SubscribeForEvents method.
	 */
	void UnsubscribeForEvents();

private:
	/**
	 * The scale factor for the map.
	 */
	const float m_mapScale;

	/**
	 * Number of rows to divide the playing surface into.
	 */
	const unsigned int m_rows;

	/**
	 * Number of columns to divide the playing surface into.
	 */
	const unsigned int m_columns;

	/**
	 * The number of lives remaining.
	 */
	int m_livesRemaining;

	/**
	 * The current score.
	 */
	int m_score;

	/**
	 * The current round number.
	 */
	int m_round;

	/**
	 * The current amount of metal remaining.
	 */
	int m_metalRemaining;

	/**
	 * Subscription identifier for the WindowResizeEvent subscription.
	 */
	Engine::EventDispatcher::SubscriptionID m_windowResizeSubscription;

	/**
	 * Subscription identifier for the MouseButtonPressedEvent subscription.
	 */
	Engine::EventDispatcher::SubscriptionID m_mouseButtonPressedSubscription;

	/**
	 * Subscription identifier for the EnemySurvivedEvent subscription.
	 */
	Engine::EventDispatcher::SubscriptionID m_enemySurvivedSubscription;

	/**
	 * Subscription identifier for the EnemyDestroyedEvent subscription.
	 */
	Engine::EventDispatcher::SubscriptionID m_enemyDestroyedSubscription;

	/**
	 * Shared pointer to the playing surface for the map.
	 */
	std::shared_ptr<PlayingSurface> m_playingSurface;

	/**
	 * Shared pointer to the game object currently being built. This will be a
	 * nullptr if no game object is currently being edited.
	 */
	std::shared_ptr<Engine::GameObject> m_unitBeingBuilt;

	/**
	 * Shared pointer to the game object currently selected. This will be a
	 * nullptr if no game object is currently selected.
	 */
	std::shared_ptr<Engine::GameObject> m_unitSelected;

	/**
	 * The metal cost of the unit currently being built;
	 */
	int m_unitCost;

	/**
	 * The rounds to be played.
	 */
	std::queue<std::unique_ptr<Round>> m_rounds;

	/**
	 * The build missile silo button UI element.
	 */
	Engine::UI::Rectangle m_buildMissileSiloButton;

	/**
	 * The build rocket launcher button UI element.
	 */
	Engine::UI::Rectangle m_buildRocketLauncherButton;

	/**
	 * The build laser tower button UI element.
	 */
	Engine::UI::Rectangle m_buildLaserTowerButton;

	/**
	 * The build wall button UI element.
	 */
	Engine::UI::Rectangle m_buildWallButton;

	/**
	 * The delete tower button UI element.
	 */
	Engine::UI::Rectangle m_deleteButton;

	/**
	 * The lives remaining icon UI element.
	 */
	Engine::UI::Rectangle m_livesRemainingIcon;

	/**
	 * The metal remaining icon UI element.
	 */
	Engine::UI::Rectangle m_metalRemainingIcon;

	/**
	 * Molot font.
	 */
	Engine::UI::Font m_molotFont;

	/**
	 * The lives remaining text label.
	 */
	Engine::UI::Text m_livesRemainingLabel;

	/**
	 * The current score text label.
	 */
	Engine::UI::Text m_scoreLabel;

	/**
	 * The current rounds text label.
	 */
	Engine::UI::Text m_roundsLabel;

	/**
	 * The current metal remaining label.
	 */
	Engine::UI::Text m_metalRemainingLabel;

	/**
	 * Background UI element for the score and rounds UI elements.
	 */
	Engine::UI::Rectangle m_centerPanelBackground;

private:
	/**
	 * The initial cell for all enemy units.
	 */
	static Cell START_CELL;

	/**
	 * The target destination cell for all enemy units.
	 */
	static Cell END_CELL;

private:
	/**
	 * The metal cost for construction of a Wall.
	 */
	static constexpr unsigned int WALL_COST = 100;

	/**
	 * The metal cost for construction of a Laser Tower.
	 */
	static constexpr unsigned int LASER_TOWER_COST = 500;

	/**
	 * The metal cost for construction of a Rocket Launcher.
	 */
	static constexpr unsigned int ROCKET_LAUNCHER_COST = 800;

	/**
	 * The metal cost for construction of a Missile Silo.
	 */
	static constexpr unsigned int MISSILE_SILO_COST = 4000;
};

#endif
