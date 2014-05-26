#include "GameScene.hpp"

#include <Engine/Ray.hpp>
#include <Engine/Audio.hpp>

#include <Engine/Event/KeyPressedEvent.hpp>
#include <Engine/Event/WindowResizeEvent.hpp>
#include <Engine/Event/DestroyGameObjectEvent.hpp>
#include <Engine/Event/PushSceneEvent.hpp>

#include <Engine/Attribute/Model.hpp>
#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/OrthographicCamera.hpp>
#include <Engine/Attribute/DirectionalLight.hpp>

#include "Event/PlayingSurfaceModifiedEvent.hpp"
#include "Event/AddTagEvent.hpp"
#include "Event/EnemySurvivedEvent.hpp"
#include "Event/EnemyDestroyedEvent.hpp"
#include "Event/ToggleRangeEvent.hpp"

#include "Attribute/Resources.hpp"

#include "ScoutFactory.hpp"
#include "TankFactory.hpp"
#include "HeavyBotFactory.hpp"
#include "PathFinder.hpp"
#include "LaserTowerFactory.hpp"
#include "RocketLauncherFactory.hpp"
#include "MissileSiloFactory.hpp"
#include "WallFactory.hpp"
#include "ExplosionFactory.hpp"

Cell GameScene::START_CELL = Cell(10, 0);
Cell GameScene::END_CELL = Cell(10, 20);

GameScene::GameScene(std::shared_ptr<Engine::Window> window,
	std::shared_ptr<Engine::ResourceManager> resourceManager,
	std::shared_ptr<Engine::EventDispatcher> sceneStackEventDispatcher)
: Engine::IGameScene(window, resourceManager, sceneStackEventDispatcher)
, m_mapScale(750.0f)
, m_rows(21)
, m_columns(21)
, m_livesRemaining(10)
, m_score(0)
, m_round(1)
, m_metalRemaining(2000)
, m_windowResizeSubscription(0)
, m_mouseButtonPressedSubscription(0)
, m_enemySurvivedSubscription(0)
, m_enemyDestroyedSubscription(0)
, m_playingSurface(std::make_shared<PlayingSurface>(glm::vec2(m_mapScale, m_mapScale), m_rows, m_columns))
, m_unitBeingBuilt(nullptr)
, m_unitSelected(nullptr)
, m_unitCost(0)
, m_rounds()
, m_buildMissileSiloButton()
, m_buildRocketLauncherButton()
, m_buildLaserTowerButton()
, m_buildWallButton()
, m_deleteButton()
, m_livesRemainingIcon()
, m_metalRemainingIcon()
, m_molotFont()
, m_livesRemainingLabel()
, m_scoreLabel()
, m_roundsLabel()
, m_metalRemainingLabel()
, m_centerPanelBackground()
{
	// Nothing to do.
}

GameScene::~GameScene()
{
	// Unsubscribe for events.
	UnsubscribeForEvents();
}

void GameScene::OnCreate()
{
	// Subscribe for events.
	SubscribeForEvents();

	// Play music.
	Engine::Audio::GetInstance().Play(GetResourceManager()->GetAudio("resources/audio/HeroicDemise.wav"));

	// Setup the "build missile silo" button UI element.
	m_buildMissileSiloButton.SetDimensions(glm::vec2(80.0f, 91.0f));
	m_buildMissileSiloButton.SetTexture(GetResourceManager()->GetTexture("resources/images/BuildMissileSiloButton.png"));

	// Setup the "build rocket launcher" button UI element.
	m_buildRocketLauncherButton.SetDimensions(glm::vec2(80.0f, 91.0f));
	m_buildRocketLauncherButton.SetTexture(GetResourceManager()->GetTexture("resources/images/BuildRocketLauncherButton.png"));

	// Setup the "build laser tower" button UI element.
	m_buildLaserTowerButton.SetDimensions(glm::vec2(80.0f, 91.0f));
	m_buildLaserTowerButton.SetTexture(GetResourceManager()->GetTexture("resources/images/BuildLaserTowerButton.png"));

	// Setup the "build wall" button UI element.
	m_buildWallButton.SetDimensions(glm::vec2(80.0f, 91.0f));
	m_buildWallButton.SetTexture(GetResourceManager()->GetTexture("resources/images/BuildWallButton.png"));

	// Setup the "delete tower" button UI element.
	m_deleteButton.SetDimensions(glm::vec2(80.0f, 91.0f));
	m_deleteButton.SetTexture(GetResourceManager()->GetTexture("resources/images/DeleteButton.png"));

	// Setup the "lives remaining" icon UI element.
	m_livesRemainingIcon.SetDimensions(glm::vec2(16.0f, 16.0f));
	m_livesRemainingIcon.SetTexture(GetResourceManager()->GetTexture("resources/images/Heart.png"));

	// Setup the "metal remaining" icon UI element.
	m_metalRemainingIcon.SetDimensions(glm::vec2(16.0f, 16.0f));
	m_metalRemainingIcon.SetTexture(GetResourceManager()->GetTexture("resources/images/Metal.png"));

	// Setup the "center panel" background UI element.
	m_centerPanelBackground.SetDimensions(glm::vec2(175.0f, 100.0f));
	m_centerPanelBackground.SetFillColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.4f));

	// Setup the font.
	if (m_molotFont.LoadFromFile("resources/fonts/Molot.otf"))
	{
		// Setup the "lives remaining" label UI element.
		m_livesRemainingLabel.SetFont(m_molotFont);
		m_livesRemainingLabel.SetCharacterSize(16);
		m_livesRemainingLabel.SetString(std::to_string(m_livesRemaining));

		// Setup the "score" label UI element.
		m_scoreLabel.SetFont(m_molotFont);
		m_scoreLabel.SetCharacterSize(36);
		m_scoreLabel.SetString(std::to_string(m_score));

		// Setup the "round" label UI element.
		m_roundsLabel.SetFont(m_molotFont);
		m_roundsLabel.SetCharacterSize(16);
		m_roundsLabel.SetString(GetRoundsLabelString());

		// Setup the "metal remaining" label UI element.
		m_metalRemainingLabel.SetFont(m_molotFont);
		m_metalRemainingLabel.SetCharacterSize(16);
		m_metalRemainingLabel.SetString(std::to_string(m_metalRemaining));
	}

	// Position all of the UI elements.
	PositionUI();

	// Create the main scene camera.
	std::shared_ptr<Engine::GameObject> camera = CreateGameObject();
	std::shared_ptr<Engine::Attribute::Transform> cameraTransform =
		camera->CreateAttribute<Engine::Attribute::Transform>();
	cameraTransform->SetPosition(glm::vec3(1000.0f, 1500.0f, 1000.0f));
	cameraTransform->SetScale(1.0f);
	cameraTransform->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	camera->CreateAttribute<Engine::Attribute::OrthographicCamera>(
		GetWindow()->GetWidth(), // Width
		GetWindow()->GetHeight(), // Height
		0.0f, // Near
		10000.0f // Far
	);

	// Set the scene camera.
	SetCamera(camera);

	// Create the main directional light.
	std::shared_ptr<Engine::GameObject> light = CreateGameObject();
	std::shared_ptr<Engine::Attribute::Transform> lightTransform =
		light->CreateAttribute<Engine::Attribute::Transform>();
	lightTransform->SetPosition(glm::vec3(0.0f, 1000.0f, 500.0f));
	lightTransform->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	light->CreateAttribute<Engine::Attribute::DirectionalLight>();

	// Create the map.
	// Note: The width and depth of the playing surface in the map are 1.0.
	// Therefore, we can scale the map by the value of PLAYING_SURFACE_SIZE
	// to ensure that the playing surface does indeed have the required size.
	std::shared_ptr<Engine::GameObject> map = CreateGameObject();
	map->CreateAttribute<Engine::Attribute::Model>("resources/maps/outlands/Map.obj");
	map->CreateAttribute<Engine::Attribute::ShaderProgram>("resources/shaders/Phong.vert", "resources/shaders/Phong.frag");
	std::shared_ptr<Engine::Attribute::Transform> mapTransform =
		map->CreateAttribute<Engine::Attribute::Transform>();
	mapTransform->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	mapTransform->SetScale(m_mapScale);

	// Setup the rounds.
	SetupRounds();
}

void GameScene::OnSuspend()
{
	// Unsubscribe for events.
	UnsubscribeForEvents();
}

void GameScene::OnResume()
{
	// Subscribe for events.
	SubscribeForEvents();
}

void GameScene::OnUpdate(double deltaTime)
{
	// Get the position of the mouse cursor.
	const glm::vec2 cursorPosition = GetWindow()->GetCursorPosition();

	// "Build missile silo" button states.
	if (m_metalRemaining < MISSILE_SILO_COST)
	{
		m_buildMissileSiloButton.SetFillColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (m_buildMissileSiloButton.ContainsPoint(cursorPosition))
	{
		m_buildMissileSiloButton.SetFillColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		m_buildMissileSiloButton.SetFillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// "Build rocket launcher" button states.
	if (m_metalRemaining < ROCKET_LAUNCHER_COST)
	{
		m_buildRocketLauncherButton.SetFillColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (m_buildRocketLauncherButton.ContainsPoint(cursorPosition))
	{
		m_buildRocketLauncherButton.SetFillColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		m_buildRocketLauncherButton.SetFillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// "Build laser tower" button states.
	if (m_metalRemaining < LASER_TOWER_COST)
	{
		m_buildLaserTowerButton.SetFillColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (m_buildLaserTowerButton.ContainsPoint(cursorPosition))
	{
		m_buildLaserTowerButton.SetFillColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		m_buildLaserTowerButton.SetFillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// "Build wall" button states.
	if (m_metalRemaining < WALL_COST)
	{
		m_buildWallButton.SetFillColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (m_buildWallButton.ContainsPoint(cursorPosition))
	{
		m_buildWallButton.SetFillColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		m_buildWallButton.SetFillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// "Delete tower" button states.
	if (m_deleteButton.ContainsPoint(cursorPosition))
	{
		m_deleteButton.SetFillColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		m_deleteButton.SetFillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// Enter the defeat scene if there are no lives remaining.
	if (m_livesRemaining <= 0)
	{
		GetSceneStackEventDispatcher()->Enqueue<Engine::Event::PushSceneEvent>("DefeatScene");
		Engine::Audio::GetInstance().StopAll();
	}

	// Update the "lives remaining" label if required.
	const std::string livesRemainingString = std::to_string(m_livesRemaining);
	if (m_livesRemainingLabel.GetString() != livesRemainingString)
	{
		m_livesRemainingLabel.SetString(livesRemainingString);

		// Reposition the UI if the "lives remaining" label string has changed.
		// This is required since the dimensions of the "lives remaining" text
		// label may have changed.
		PositionUI();
	}

	// Update the "score" label if required.
	const std::string scoreString = std::to_string(m_score);
	if (m_scoreLabel.GetString() != scoreString)
	{
		m_scoreLabel.SetString(scoreString);

		// Reposition the UI if the "score" label string has changed. This is
		// required since the dimensions of the "score" text label may have
		// changed.
		PositionUI();
	}

	// Update the "metal remaining" label if required.
	const std::string metalRemainingString = std::to_string(m_metalRemaining);
	if (m_metalRemainingLabel.GetString() != metalRemainingString)
	{
		m_metalRemainingLabel.SetString(metalRemainingString);

		// Reposition the UI if the "metal remaining" label string has changed.
		// This is required since the dimensions of the "metal remaining" text
		// label may have changed.
		PositionUI();
	}

	// Move the unit being built to the position of the mouse cursor.
	if (m_unitBeingBuilt)
	{
		// Determine the grid cell that the cursor is over.
		Cell cell = ScreenPointToCell(cursorPosition);

		// Move the unit to the centroid position of the cell.
		m_unitBeingBuilt->GetAttribute<Engine::Attribute::Transform>()->SetPosition(m_playingSurface->GetPositionForCell(cell));
	}

	// Check whether there are still rounds to play.
	if (!m_rounds.empty())
	{
		// Update the current round.
		m_rounds.front()->Update(deltaTime);

		// Check whether the current round is finished.
		// If it is, move to the next round.
		if (m_rounds.front()->IsFinished())
		{
			m_rounds.pop();
			m_round += 1;
		}
	}
	else
	{
		// If there are no rounds to play, then the level is complete.
		GetSceneStackEventDispatcher()->Enqueue<Engine::Event::PushSceneEvent>("VictoryScene");
		Engine::Audio::GetInstance().StopAll();
	}

	// Update the "rounds" label if required.
	const std::string roundsLabelString = GetRoundsLabelString();
	if (m_roundsLabel.GetString() != roundsLabelString)
	{
		m_roundsLabel.SetString(roundsLabelString);

		// Reposition the UI if the "rounds" label string has changed. This is
		// required since the dimensions of the "rounds" text label may have
		// changed.
		PositionUI();
	}
}

void GameScene::OnDrawUI()
{
	// Get the UI shader.
	std::shared_ptr<Engine::ShaderProgram> uiShader = GetResourceManager()->GetShaderProgram("resources/shaders/UI.vert", "resources/shaders/UI.frag");

	// Draw the build tower buttons.
	DrawShape(m_buildMissileSiloButton, uiShader);
	DrawShape(m_buildRocketLauncherButton, uiShader);
	DrawShape(m_buildLaserTowerButton, uiShader);
	DrawShape(m_buildWallButton, uiShader);

	// Draw the delete tower button if a tower is currently selected.
	if (m_unitSelected)
	{
		DrawShape(m_deleteButton, uiShader);
	}

	// Draw the lives remaining icon.
	DrawShape(m_livesRemainingIcon, uiShader);

	// Draw the lives remaining label.
	DrawShape(m_livesRemainingLabel, uiShader);

	// Draw the center planel background.
	DrawShape(m_centerPanelBackground, uiShader);

	// Draw the score label.
	DrawShape(m_scoreLabel, uiShader);

	// Draw the rounds label.
	DrawShape(m_roundsLabel, uiShader);

	// Draw the metal remaining icon.
	DrawShape(m_metalRemainingIcon, uiShader);

	// Draw the metal remaining label.
	DrawShape(m_metalRemainingLabel, uiShader);
}

void GameScene::HandleMouseButtonPressedEvent(const Engine::Event::MouseButtonPressedEvent& event)
{
	// Get the coordinates of the button press.
	const glm::vec2 coordinates = event.GetCoordinates();

	// Check whether any of the UI elements were clicked.
	// If one of the buttons was clicked, check that we are not already
	// constructing a unit and that there is sufficient metal in reserve to
	// build the requested unit.
	if (m_buildMissileSiloButton.ContainsPoint(coordinates))
	{
		if (!m_unitBeingBuilt && m_metalRemaining >= MISSILE_SILO_COST)
		{
			MissileSiloFactory factory(m_playingSurface);
			m_unitBeingBuilt = CreateGameObject(factory);
			m_unitBeingBuilt->BroadcastEnqueue<Event::ToggleRangeEvent>(true);
			m_unitCost = MISSILE_SILO_COST;
		}
	}
	else if (m_buildRocketLauncherButton.ContainsPoint(coordinates))
	{
		if (!m_unitBeingBuilt && m_metalRemaining >= ROCKET_LAUNCHER_COST)
		{
			RocketLauncherFactory factory(m_playingSurface);
			m_unitBeingBuilt = CreateGameObject(factory);
			m_unitBeingBuilt->BroadcastEnqueue<Event::ToggleRangeEvent>(true);
			m_unitCost = ROCKET_LAUNCHER_COST;
		}
	}
	else if (m_buildLaserTowerButton.ContainsPoint(coordinates))
	{
		if (!m_unitBeingBuilt && m_metalRemaining >= LASER_TOWER_COST)
		{
			LaserTowerFactory factory(m_playingSurface);
			m_unitBeingBuilt = CreateGameObject(factory);
			m_unitBeingBuilt->BroadcastEnqueue<Event::ToggleRangeEvent>(true);
			m_unitCost = LASER_TOWER_COST;
		}
	}
	else if (m_buildWallButton.ContainsPoint(coordinates))
	{
		if (!m_unitBeingBuilt && m_metalRemaining >= WALL_COST)
		{
			WallFactory factory;
			m_unitBeingBuilt = CreateGameObject(factory);
			m_unitBeingBuilt->BroadcastEnqueue<Event::ToggleRangeEvent>(true);
			m_unitCost = WALL_COST;
		}
	}
	else if (m_deleteButton.ContainsPoint(coordinates) && m_unitSelected)
	{
		// Vacate the cell that the selected tower currently occupies.
		assert(m_unitSelected->HasAttribute<Engine::Attribute::Transform>());
		const Cell cell = m_playingSurface->GetCellForPosition(m_unitSelected->GetAttribute<Engine::Attribute::Transform>()->GetPosition());
		m_playingSurface->VacateCell(cell);

		// Destroy the selected tower.
		m_unitSelected->EnqueueEvent<Engine::Event::DestroyGameObjectEvent>();
		m_unitSelected = nullptr;

		// Raise a PlayingSurfaceModifiedEvent so that game objects
		// can take relevant action (for things like pathfinding).
		GetEventDispatcher()->Enqueue<Event::PlayingSurfaceModifiedEvent>(*m_playingSurface);
	}
	else
	{
		// Determine the grid cell that the cursor is over.
		const Cell cell = ScreenPointToCell(coordinates);

		// Stop selection of the currently selected tower.
		if (m_unitSelected)
		{
			m_unitSelected->BroadcastEnqueue<Event::ToggleRangeEvent>(false);
			m_unitSelected = nullptr;
		}

		// If non of the UI elements were clicked, then the scene is assumed to
		// have been clicked. If a unit is currently being build, we place that
		// unit at the centroid position of the cell corresponding to the mouse
		// click coordinates.
		if (m_unitBeingBuilt)
		{
			// We only place the unit if it is within the range of the playing
			// grid, the cell is not already occupied. Otherwise, we cancel
			// construction of the unit.
			if (m_playingSurface->ContainsCell(cell) && !m_playingSurface->GetOccupant(cell))
			{
				// Create a copy of the Playing Surface with the determined cell
				// occupied.
				PlayingSurface playingSurfaceCopy = *m_playingSurface;
				playingSurfaceCopy.OccupyCell(cell, m_unitBeingBuilt);

				// Try to find a path from the starting cell to the ending cell
				// in the modified copy of the playing surface.
				const Path path = PathFinder::ShortestPath(playingSurfaceCopy, START_CELL, END_CELL);

				// Ensure that a path from the starting cell to the ending cell
				// exists.
				if (cell != START_CELL && cell != END_CELL && path.GetLength() > 0)
				{
					// Set the unit's final position.
					if (m_unitBeingBuilt->HasAttribute<Engine::Attribute::Transform>())
					{
						std::shared_ptr<Engine::Attribute::Transform> transform = m_unitBeingBuilt->GetAttribute<Engine::Attribute::Transform>();
						transform->SetPosition(m_playingSurface->GetPositionForCell(cell));
					}

					// Request to add an "Enabled" tag to the unit and all child
					// Game Objects.
					m_unitBeingBuilt->BroadcastEnqueue<Event::AddTagEvent>("Enabled");

					// Stop displaying the range of the tower.
					m_unitBeingBuilt->BroadcastEnqueue<Event::ToggleRangeEvent>(false);

					// Mark cell corresponding to the position of the unit as
					// occupied in the playing surface.
					m_playingSurface->OccupyCell(cell, m_unitBeingBuilt);

					// Deduct the unit cost from the remaining metal.
					m_metalRemaining -= m_unitCost;

					// Raise a PlayingSurfaceModifiedEvent so that game objects
					// can take relevant action (for things like pathfinding).
					GetEventDispatcher()->Enqueue<Event::PlayingSurfaceModifiedEvent>(*m_playingSurface);
				}
				else
				{
					m_unitBeingBuilt->EnqueueEvent<Engine::Event::DestroyGameObjectEvent>();
				}
			}
			else
			{
				m_unitBeingBuilt->EnqueueEvent<Engine::Event::DestroyGameObjectEvent>();
			}

			// Stop construction of the unit.
			m_unitBeingBuilt = nullptr;
			m_unitCost = 0;
		}
		else
		{
			// No unit is currently being constructed. Let's check whether an
			// already placed unit was clicked.
			if (m_playingSurface->ContainsCell(cell) && m_playingSurface->GetOccupant(cell))
			{
				// Record the unit as being selected.
				m_unitSelected = m_playingSurface->GetOccupant(cell);

				// Start displaying the range of the tower that was clicked.
				m_unitSelected->BroadcastEnqueue<Event::ToggleRangeEvent>(true);
			}
		}
	}
}

std::string GameScene::GetRoundsLabelString() const
{
	return "Round " + std::to_string(m_round) + " / " + std::to_string(m_round + m_rounds.size() - 1);
}

Cell GameScene::ScreenPointToCell(const glm::vec2& screenPoint) const
{
	const Engine::Plane xzPlane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	const Engine::Ray ray = ScreenPointToRay(screenPoint);
	float intersectionDistance;
	if (xzPlane.Raycast(ray, intersectionDistance))
	{
		const glm::vec3 intersectionPoint = ray.GetPoint(intersectionDistance);
		return m_playingSurface->GetCellForPosition(intersectionPoint);
	}
	else
	{
		// Woah! The ray didn't intersect the xz-axis plane. This should
		// only happen if the camera is not facing in the direction of the
		// xz-axis.
		assert(0);
		return Cell(0, 0);
	}
}

void GameScene::PositionUI()
{
	const glm::vec2 windowDimensions = GetWindow()->GetDimensions();

	// Set the position of the "build missile silo" button.
	m_buildMissileSiloButton.SetPosition(windowDimensions - m_buildRocketLauncherButton.GetDimensions() - glm::vec2(25.0f, 25.0f));

	// Set the position of the "build rocket launcher" button.
	m_buildRocketLauncherButton.SetPosition(m_buildMissileSiloButton.GetPosition() - glm::vec2(m_buildRocketLauncherButton.GetWidth() + 25.0f, 0.0f));

	// Set the position of the "Build laser tower" button.
	m_buildLaserTowerButton.SetPosition(m_buildRocketLauncherButton.GetPosition() - glm::vec2(m_buildLaserTowerButton.GetWidth() + 25.0f, 0.0f));

	// Set the position of the "build wall" button.
	m_buildWallButton.SetPosition(m_buildLaserTowerButton.GetPosition() - glm::vec2(m_buildWallButton.GetWidth() + 25.0f, 0.0f));

	// Set the position of the "delete tower" button.
	m_deleteButton.SetPosition(glm::vec2(25.0f, m_buildWallButton.GetPosition().y));

	// Set the position of the "lives remaining" icon.
	m_livesRemainingIcon.SetPosition(glm::vec2(
		windowDimensions.x - m_livesRemainingIcon.GetWidth() - 50.0f,
		25.0f
	));

	// Set the position of the "lives remaining" label.
	m_livesRemainingLabel.SetPosition(m_livesRemainingIcon.GetPosition()
		+ glm::vec2(m_livesRemainingIcon.GetWidth() + 10.0f, -3.0f));

	// Set the position of the "score" label.
	m_scoreLabel.SetPosition(glm::vec2(
		0.5f * windowDimensions.x - 0.5f * m_scoreLabel.GetWidth(),
		10.0f
	));

	// Set the position of the "round" label.
	m_roundsLabel.SetPosition(glm::vec2(
		0.5f * windowDimensions.x - 0.5f * m_roundsLabel.GetWidth(),
		m_scoreLabel.GetPosition().y + m_scoreLabel.GetHeight() + 10.0f
	));

	// Set the position of the "metal remaining" icon.
	m_metalRemainingIcon.SetPosition(glm::vec2(25.0f, 25.0f));

	// Set the position of the "metal remaining" label.
	m_metalRemainingLabel.SetPosition(m_metalRemainingIcon.GetPosition() +
		glm::vec2(m_metalRemainingIcon.GetWidth() + 10.0f, -3.0f));

	// Set the position of the "center panel" background.
	m_centerPanelBackground.SetPosition(glm::vec2(
		0.5f * windowDimensions.x - 0.5f * m_centerPanelBackground.GetWidth(),
		0.0f
	));
}

void GameScene::SetupRounds()
{
	// Generate the first five rounds.
	for (unsigned int i = 0; i < 5; ++i)
	{
		// Create the enemy unit factories for these rounds.
		std::shared_ptr<ScoutFactory> scoutFactory =
			std::make_shared<ScoutFactory>(m_playingSurface, START_CELL, END_CELL, 1.0f + 0.75f * i);

		// Determine the number of each type of unit.
		const unsigned int numScouts = 4 + i * 2;

		// Determine the round duration.
		const double duration = 3.0 * numScouts;

		// Create the round.
		std::unique_ptr<Round> round(new Round(GetEventDispatcher(), duration));

		// Populate the round.
		for (unsigned int j = 0; j < numScouts; ++j)
		{
			round->Spawn(scoutFactory);
		}

		// Add the round to the queue.
		m_rounds.push(std::move(round));
	}

	// Generate the next five rounds.
	for (unsigned int i = 0; i < 5; ++i)
	{
		// Create the enemy unit factories for these rounds.
		std::shared_ptr<ScoutFactory> scoutFactory =
			std::make_shared<ScoutFactory>(m_playingSurface, START_CELL, END_CELL, 4.0f + 0.75f * i);
		std::shared_ptr<HeavyBotFactory> heavyBotFactory =
			std::make_shared<HeavyBotFactory>(m_playingSurface, START_CELL, END_CELL, 1.0 + 0.125f * i);

		// Determine the number of each type of unit.
		const unsigned int numScouts = 4 + i * 2;
		const unsigned int numHeavyBots = 1 + i;

		// Calculate the total number of units.
		const unsigned int numUnits = numScouts + numHeavyBots;

		// Determine the round duration.
		const double duration = 3.0 * numUnits;

		// Create the round.
		std::unique_ptr<Round> round(new Round(GetEventDispatcher(), duration));

		// Populate the round.
		for (unsigned int j = 0; j < numScouts; ++j)
		{
			round->Spawn(scoutFactory);
		}
		for (unsigned int j = 0; j < numHeavyBots; ++j)
		{
			round->Spawn(heavyBotFactory);
		}

		// Add the round to the queue.
		m_rounds.push(std::move(round));
	}

	// Generate the next five rounds.
	for (unsigned int i = 0; i < 5; ++i)
	{
		// Create the enemy unit factories for these rounds.
		std::shared_ptr<ScoutFactory> scoutFactory =
			std::make_shared<ScoutFactory>(m_playingSurface, START_CELL, END_CELL, 4.0f + 0.75f * i);
		std::shared_ptr<HeavyBotFactory> heavyBotFactory =
			std::make_shared<HeavyBotFactory>(m_playingSurface, START_CELL, END_CELL, 1.0 + 0.125f * i);

		// Determine the number of each type of unit.
		const unsigned int numScouts = 4 + i * 2;
		const unsigned int numHeavyBots = 2 + i * 2;

		// Calculate the total number of units.
		const unsigned int numUnits = numScouts + numHeavyBots;

		// Determine the round duration.
		const double duration = 1.0 * numUnits;

		// Create the round.
		std::unique_ptr<Round> round(new Round(GetEventDispatcher(), duration));

		// Populate the round.
		for (unsigned int j = 0; j < numScouts; ++j)
		{
			round->Spawn(scoutFactory);
		}
		for (unsigned int j = 0; j < numHeavyBots; ++j)
		{
			round->Spawn(heavyBotFactory);
		}

		// Add the round to the queue.
		m_rounds.push(std::move(round));
	}

	// Generate the next five rounds.
	for (unsigned int i = 0; i < 5; ++i)
	{
		// Create the enemy unit factories for these rounds.
		std::shared_ptr<HeavyBotFactory> heavyBotFactory =
			std::make_shared<HeavyBotFactory>(m_playingSurface, START_CELL, END_CELL, 1.0 + 0.125f * i);
		std::shared_ptr<TankFactory> tankFactory =
			std::make_shared<TankFactory>(m_playingSurface, START_CELL, END_CELL, 1.0 + 0.125f * i);

		// Determine the number of each type of unit.
		const unsigned int numHeavyBots = 2 + i * 2;
		const unsigned int numTanks = 1 + i;

		// Calculate the total number of units.
		const unsigned int numUnits = numHeavyBots + numTanks;

		// Determine the round duration.
		const double duration = 2.0 * numUnits;

		// Create the round.
		std::unique_ptr<Round> round(new Round(GetEventDispatcher(), duration));

		// Populate the round.
		for (unsigned int j = 0; j < numHeavyBots; ++j)
		{
			round->Spawn(heavyBotFactory);
		}
		for (unsigned int j = 0; j < numTanks; ++j)
		{
			round->Spawn(tankFactory);
		}

		// Add the round to the queue.
		m_rounds.push(std::move(round));
	}
}

void GameScene::SubscribeForEvents()
{
	// Mouse button pressed events.
	m_mouseButtonPressedSubscription = GetWindow()->GetEventDispatcher()->Subscribe<Engine::Event::MouseButtonPressedEvent>(CALLBACK(GameScene::HandleMouseButtonPressedEvent));

	// Window resize events.
	m_windowResizeSubscription = GetWindow()->GetEventDispatcher()->Subscribe<Engine::Event::WindowResizeEvent>(
		[this](const Engine::Event::WindowResizeEvent& event) {
			// Get the new window width and height (in pixels).
			const int width = event.GetWidth();
			const int height = event.GetHeight();

			// Update the camera's projection.
			std::shared_ptr<Engine::Attribute::OrthographicCamera> cameraProjection =
				GetCamera()->GetAttribute<Engine::Attribute::OrthographicCamera>();
			cameraProjection->SetViewportWidth(width);
			cameraProjection->SetViewportHeight(height);

			// Calculate the zoom factor.
			const float scaleX = static_cast<float>(width) / 1280.0f;
			const float scaleY = static_cast<float>(height) / 900.0f;
			const float scale = std::min(scaleX, scaleY);

			// Zoom the camera.
			std::shared_ptr<Engine::Attribute::Transform> cameraTransform =
				GetCamera()->GetAttribute<Engine::Attribute::Transform>();
			cameraTransform->SetScale(1.0f / scale);

			// Reposition all the UI elements.
			PositionUI();
		}
	);

	// Enemy survived events.
	m_enemySurvivedSubscription = GetEventDispatcher()->Subscribe<Event::EnemySurvivedEvent>(
		[this] (const Event::EnemySurvivedEvent& event) {
			// Decrement the number of lives remaining.
			m_livesRemaining -= 1;
		}
	);

	// Enemy destroyed events.
	m_enemyDestroyedSubscription = GetEventDispatcher()->Subscribe<Event::EnemyDestroyedEvent>(
		[this] (const Event::EnemyDestroyedEvent& event) {
			// Get the destroyed Game Object.
			std::shared_ptr<Engine::GameObject> destroyedEnemy = event.GetGameObject();

			// Increase the score.
			m_score += 500;

			// Increase the amount of metal held in reserve by the amount of
			// metal carried by the destroyed Game Object.
			if (destroyedEnemy->HasAttribute<Attribute::Resources>())
			{
				m_metalRemaining += destroyedEnemy->GetAttribute<Attribute::Resources>()->GetMetal();
			}

			// Determine the position for the explosion.
			if (destroyedEnemy->HasAttribute<Engine::Attribute::Transform>())
			{
				// Create an explosion at the position of the Game Object.
				std::shared_ptr<Engine::Attribute::Transform> transform =
					destroyedEnemy->GetAttribute<Engine::Attribute::Transform>();
				ExplosionFactory factory;
				std::shared_ptr<Engine::GameObject> explosion = CreateGameObject(factory);
				explosion->GetAttribute<Engine::Attribute::Transform>()->SetPosition(transform->GetPosition());
			}
		}
	);
}

void GameScene::UnsubscribeForEvents()
{
	// Mouse button pressed events.
	GetWindow()->GetEventDispatcher()->Unsubscribe<Engine::Event::MouseButtonPressedEvent>(m_mouseButtonPressedSubscription);

	// Window resize events.
	GetWindow()->GetEventDispatcher()->Unsubscribe<Engine::Event::WindowResizeEvent>(m_windowResizeSubscription);

	// Enemy survived events.
	GetEventDispatcher()->Unsubscribe<Event::EnemySurvivedEvent>(m_enemySurvivedSubscription);

	// Enemy destroyed events.
	GetEventDispatcher()->Unsubscribe<Event::EnemyDestroyedEvent>(m_enemyDestroyedSubscription);
}
