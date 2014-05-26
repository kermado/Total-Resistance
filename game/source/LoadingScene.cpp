#include "LoadingScene.hpp"

#include <iostream>

#include <Engine/ShaderProgram.hpp>
#include <Engine/Texture.hpp>
#include <Engine/Audio.hpp>
#include <Engine/IAudioSource.hpp>

#include <Engine/Event/ResourceLoadedEvent.hpp>
#include <Engine/Event/WindowResizeEvent.hpp>
#include <Engine/Event/PushSceneEvent.hpp>

LoadingScene::LoadingScene(std::shared_ptr<Engine::Window> window,
	std::shared_ptr<Engine::ResourceManager> resourceManager,
	std::shared_ptr<Engine::EventDispatcher> sceneStackEventDispatcher)
: IScene(window, resourceManager, sceneStackEventDispatcher)
, m_loadingBar()
, m_startButton()
, m_titleImage()
, m_starsBackground1()
, m_starsBackground2()
, m_nebulaBackground1()
, m_nebulaBackground2()
, m_mouseButtonPressedSubscription(0)
, m_windowResizeSubscription(0)
, m_resourceLoadingRequestCount(0)
, m_resourceLoadingCompleteCount(0)
{
	// Nothing to do.
}

LoadingScene::~LoadingScene()
{
	// Unsubscribe for events.
	UnsubscribeForEvents();
}

void LoadingScene::OnCreate()
{
	// Subscribe for events.
	SubscribeForEvents();

	// Load shader for rendering the User Interface.
	GetResourceManager()->LoadShaderProgram(
		"resources/shaders/UI.vert",
		"resources/shaders/UI.frag",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::ShaderProgram>& event)
		{
			// Nothing to do.
		}
	);

	// Load textures for the scene background.
	GetResourceManager()->LoadTexture(
		"resources/images/LoadingSceneBackgroundStars.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			event.GetResource()->SetRepeat(true);
			m_starsBackground1.SetTexture(event.GetResource());
			m_starsBackground2.SetTexture(event.GetResource());
		}
	);
	GetResourceManager()->LoadTexture(
		"resources/images/LoadingSceneBackgroundNebula.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			event.GetResource()->SetRepeat(true);
			m_nebulaBackground1.SetTexture(event.GetResource());
			m_nebulaBackground2.SetTexture(event.GetResource());
		}
	);

	// Load texture for the start game button.
	GetResourceManager()->LoadTexture(
		"resources/images/StartGameButton.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			m_startButton.SetTexture(event.GetResource());
		}
	);

	// Load texture for the title image.
	GetResourceManager()->LoadTexture(
		"resources/images/Title.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			m_titleImage.SetTexture(event.GetResource());
			m_titleImage.SetFillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	);

	// Setup loading bar UI element.
	m_loadingBar.SetPosition(glm::vec2(0.0f, 0.0f));
	m_loadingBar.SetDimensions(glm::vec2(GetWindow()->GetWidth(), 4.0f));
	m_loadingBar.SetFillColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	// Setup the start game button UI element.
	m_startButton.SetDimensions(START_BUTTON_DIMENSIONS);

	// Setup the title image UI element.
	m_titleImage.SetDimensions(TITLE_IMAGE_DIMENSIONS);
	m_titleImage.SetFillColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));

	// Setup the stars background UI elements.
	m_starsBackground1.SetPosition(glm::vec2(0.0f, 0.0f));
	m_starsBackground1.SetDimensions(glm::vec2(BACKGROUND_WIDTH, GetWindow()->GetHeight()));
	m_starsBackground2.SetPosition(glm::vec2(glm::vec2(BACKGROUND_WIDTH, 0.0f)));
	m_starsBackground2.SetDimensions(glm::vec2(BACKGROUND_WIDTH, GetWindow()->GetHeight()));

	// Setup the nebula background UI elements.
	m_nebulaBackground1.SetPosition(glm::vec2(0.0f, 0.0f));
	m_nebulaBackground1.SetDimensions(glm::vec2(BACKGROUND_WIDTH, GetWindow()->GetHeight()));
	m_nebulaBackground2.SetPosition(glm::vec2(glm::vec2(BACKGROUND_WIDTH, 0.0f)));
	m_nebulaBackground2.SetDimensions(glm::vec2(BACKGROUND_WIDTH, GetWindow()->GetHeight()));

	// Position UI elements.
	PositionUI();

	// Load music for this scene.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadAudio(
		"resources/audio/Soliloquy.wav",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::IAudioSource>& event)
		{
			m_resourceLoadingCompleteCount += 1;
			Engine::Audio::GetInstance().Play(event.GetResource());
		}
	);

	// Load music for the Game Scene.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadAudio(
		"resources/audio/HeroicDemise.wav",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::IAudioSource>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load sound effects for this scene.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadAudio(
		"resources/audio/ButtonSelect.wav",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::IAudioSource>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load sound effects for the Game Scene.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadAudio(
		"resources/audio/SmallExplosion.wav",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::IAudioSource>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadAudio(
		"resources/audio/RocketExplosion.wav",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::IAudioSource>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadAudio(
		"resources/audio/RocketLaunch.wav",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::IAudioSource>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadAudio(
		"resources/audio/MissileExplosion.wav",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::IAudioSource>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadAudio(
		"resources/audio/MissileLaunch.wav",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::IAudioSource>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Shader programs to load.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadShaderProgram("resources/shaders/Phong.vert", "resources/shaders/Phong.frag",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::ShaderProgram>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadShaderProgram("resources/shaders/Phong.vert", "resources/shaders/Cloud.frag",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::ShaderProgram>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load the tank model.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/tank/Tank.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load heavy bot model.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/heavybot/AnimatedHeavyBot.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load scout model.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/scout/Scout.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load the tower range model.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/range/Range.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load the laser tower models.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/lasertower/LaserTowerBase.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/lasertower/LaserTowerTurret.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/lasertower/Laser.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load the rocket launcher models.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/rocketlauncher/RocketLauncherBase.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/rocketlauncher/RocketLauncherTurret.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/rocketlauncher/Rocket.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load the wall model.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/wall/Wall.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load the missile silo models.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/missilesilo/MissileSilo.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/missilesilo/Missile.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load the explosion cloud model.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/models/explosion/Cloud.dae",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load the map.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadModel("resources/maps/outlands/Map.obj",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Model>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load texture for the "build laser tower" button.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadTexture("resources/images/BuildLaserTowerButton.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load texture for the "build rocket launcher" button.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadTexture("resources/images/BuildRocketLauncherButton.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load texture for the "delete tower" button.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadTexture("resources/images/DeleteButton.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load texture for the "build wall" button.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadTexture("resources/images/BuildWallButton.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load texture for the "build missile silo" button.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadTexture("resources/images/BuildMissileSiloButton.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load texture for the "lives remaining" icon.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadTexture("resources/images/Heart.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load texture for the "metal remaining" icon.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadTexture("resources/images/Metal.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			m_resourceLoadingCompleteCount += 1;
		}
	);

	// Load textures used by the map.
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadTexture("resources/maps/outlands/HeavyDutyConcrete.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			event.GetResource()->SetSmooth(true);
			event.GetResource()->SetRepeat(true);
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadTexture("resources/maps/outlands/Asphalt.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			event.GetResource()->SetSmooth(true);
			event.GetResource()->SetRepeat(true);
			m_resourceLoadingCompleteCount += 1;
		}
	);
	m_resourceLoadingRequestCount += 1;
	GetResourceManager()->LoadTexture("resources/maps/outlands/TerrainTexture.png",
		[this](const Engine::Event::ResourceLoadedEvent<Engine::Texture>& event)
		{
			event.GetResource()->SetSmooth(true);
			event.GetResource()->SetRepeat(true);
			m_resourceLoadingCompleteCount += 1;
		}
	);
}

void LoadingScene::OnSuspend()
{
	// Unsubscribe for events.
	UnsubscribeForEvents();
}

void LoadingScene::OnResume()
{
	// Subscribe for events.
	SubscribeForEvents();

	// Start playing music again.
	Engine::Audio::GetInstance().StopAll();
	Engine::Audio::GetInstance().Play(GetResourceManager()->GetAudio("resources/audio/Soliloquy.wav"));
}

void LoadingScene::OnUpdate(double deltaTime)
{
	// Calculate the fraction completion.
	assert(m_resourceLoadingRequestCount > 0);
	const float completion = static_cast<float>(m_resourceLoadingCompleteCount) / m_resourceLoadingRequestCount;

	// Update the loading bar.
	m_loadingBar.SetWidth(GetWindow()->GetWidth() * completion);

	// Move the background elements.
	const float starsBackgroundSpeed = -20.0f;
	const float nebulaBackgroundSpeed = -60.0f;
	m_starsBackground1.Translate(glm::vec2(starsBackgroundSpeed * deltaTime, 0.0f));
	m_starsBackground2.Translate(glm::vec2(starsBackgroundSpeed * deltaTime, 0.0f));
	m_nebulaBackground1.Translate(glm::vec2(nebulaBackgroundSpeed * deltaTime, 0.0f));
	m_nebulaBackground2.Translate(glm::vec2(nebulaBackgroundSpeed * deltaTime, 0.0f));

	// Move the 1st stars background element to the right of the screen if it is
	// no longer in view.
	if (m_starsBackground1.GetPosition().x <= -BACKGROUND_WIDTH)
	{
		m_starsBackground1.SetPosition(m_starsBackground2.GetPosition() + glm::vec2(BACKGROUND_WIDTH, 0.0f));
	}

	// Move the 2nd stars background element to the right of the screen if it is
	// no longer in view.
	if (m_starsBackground2.GetPosition().x <= -BACKGROUND_WIDTH)
	{
		m_starsBackground2.SetPosition(m_starsBackground1.GetPosition() + glm::vec2(BACKGROUND_WIDTH, 0.0f));
	}

	// Move the 1st nebula background element to the right of the screen if it
	// is no longer in view.
	if (m_nebulaBackground1.GetPosition().x <= -BACKGROUND_WIDTH)
	{
		m_nebulaBackground1.SetPosition(m_nebulaBackground2.GetPosition() + glm::vec2(BACKGROUND_WIDTH, 0.0f));
	}

	// Move the 2nd nebula background element to the right of the screen if it
	// is no longer in view.
	if (m_nebulaBackground2.GetPosition().x <= -BACKGROUND_WIDTH)
	{
		m_nebulaBackground2.SetPosition(m_nebulaBackground1.GetPosition() + glm::vec2(BACKGROUND_WIDTH, 0.0f));
	}

	// Start game button hover state.
	if (m_startButton.ContainsPoint(GetWindow()->GetCursorPosition()))
	{
		m_startButton.SetFillColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.8f));
	}
	else
	{
		m_startButton.SetFillColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));
	}
}

void LoadingScene::OnDrawUI()
{
	// Get the UI shader.
	std::shared_ptr<Engine::ShaderProgram> uiShader = GetResourceManager()->GetShaderProgram("resources/shaders/UI.vert", "resources/shaders/UI.frag");

	// Draw the background elements first.
	DrawShape(m_starsBackground1, uiShader);
	DrawShape(m_starsBackground2, uiShader);
	DrawShape(m_nebulaBackground1, uiShader);
	DrawShape(m_nebulaBackground2, uiShader);

	// Now draw either the loading bar or the start game button, depending on
	// whether all of the resources have been loaded.
	if (m_resourceLoadingCompleteCount != m_resourceLoadingRequestCount)
	{
		DrawShape(m_loadingBar, uiShader);
	}
	else
	{
		DrawShape(m_startButton, uiShader);
	}

	// Draw the title image.
	DrawShape(m_titleImage, uiShader);
}

void LoadingScene::HandleMouseButtonPressedEvent(const Engine::Event::MouseButtonPressedEvent& event)
{
	if (m_startButton.ContainsPoint(event.GetCoordinates()))
	{
		Engine::Audio::GetInstance().StopAll();
		Engine::Audio::GetInstance().Play(GetResourceManager()->GetAudio("resources/audio/ButtonSelect.wav"));
		GetSceneStackEventDispatcher()->Enqueue<Engine::Event::PushSceneEvent>("MainGameScene");
	}
}

void LoadingScene::PositionUI()
{
	m_startButton.SetPosition(GetWindow()->GetDimensions() * 0.5f  - START_BUTTON_DIMENSIONS * 0.5f + glm::vec2(0.0f, 60.0f));
	m_titleImage.SetPosition(GetWindow()->GetDimensions() * 0.5f  - TITLE_IMAGE_DIMENSIONS * 0.5f - glm::vec2(0.0f, 60.0f));
}

void LoadingScene::SubscribeForEvents()
{
	// Mouse button pressed events.
	m_mouseButtonPressedSubscription = GetWindow()->GetEventDispatcher()->Subscribe<Engine::Event::MouseButtonPressedEvent>(CALLBACK(LoadingScene::HandleMouseButtonPressedEvent));

	// Window resize events.
	m_windowResizeSubscription = GetWindow()->GetEventDispatcher()->Subscribe<Engine::Event::WindowResizeEvent>(
		[this](const Engine::Event::WindowResizeEvent& event) {
			// Resize background UI elements to fill the height of the window.
			m_starsBackground1.SetHeight(event.GetHeight());
			m_starsBackground2.SetHeight(event.GetHeight());
			m_nebulaBackground1.SetHeight(event.GetHeight());
			m_nebulaBackground2.SetHeight(event.GetHeight());

			// Reposition UI elements.
			PositionUI();
		}
	);
}

void LoadingScene::UnsubscribeForEvents()
{
	// Mouse button pressed events.
	GetWindow()->GetEventDispatcher()->Unsubscribe<Engine::Event::MouseButtonPressedEvent>(m_mouseButtonPressedSubscription);

	// Window resize events.
	GetWindow()->GetEventDispatcher()->Unsubscribe<Engine::Event::WindowResizeEvent>(m_windowResizeSubscription);
}

