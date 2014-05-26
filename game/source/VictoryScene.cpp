#include "VictoryScene.hpp"

#include <Engine/Event/PopSceneEvent.hpp>
#include <Engine/Event/WindowResizeEvent.hpp>

VictoryScene::VictoryScene(std::shared_ptr<Engine::Window> window,
	std::shared_ptr<Engine::ResourceManager> resourceManager,
	std::shared_ptr<Engine::EventDispatcher> sceneStackEventDispatcher)
: Engine::IScene(window, resourceManager, sceneStackEventDispatcher)
, m_molotFont()
, m_victoryLabel()
, m_timeRemaining(5.0f)
, m_windowResizeSubscription(0)
{
	// Nothing to do.
}

VictoryScene::~VictoryScene()
{
	UnsubscribeForEvents();
}

void VictoryScene::OnCreate()
{
	SubscribeForEvents();

	if (m_molotFont.LoadFromFile("resources/fonts/Molot.otf"))
	{
		m_victoryLabel.SetFont(m_molotFont);
		m_victoryLabel.SetCharacterSize(60);
		m_victoryLabel.SetString("Victory");
	}

	PositionUI();
}

void VictoryScene::OnSuspend()
{
	UnsubscribeForEvents();
}

void VictoryScene::OnResume()
{
	SubscribeForEvents();
}

void VictoryScene::OnUpdate(double deltaTime)
{
	m_timeRemaining -= deltaTime;

	if (m_timeRemaining < 0)
	{
		// Pop this scene and the Game Scene off the scene stack.
		GetSceneStackEventDispatcher()->Enqueue<Engine::Event::PopSceneEvent>();
		GetSceneStackEventDispatcher()->Enqueue<Engine::Event::PopSceneEvent>();
	}
}

void VictoryScene::OnDrawUI()
{
	// Get the UI shader.
	std::shared_ptr<Engine::ShaderProgram> uiShader =
		GetResourceManager()->GetShaderProgram(
			"resources/shaders/UI.vert",
			"resources/shaders/UI.frag"
		);

	// Draw the victory label.
	DrawShape(m_victoryLabel, uiShader);
}

void VictoryScene::PositionUI()
{
	m_victoryLabel.SetPosition(GetWindow()->GetDimensions() * 0.5f - m_victoryLabel.GetDimensions() * 0.5f);
}

void VictoryScene::SubscribeForEvents()
{
	m_windowResizeSubscription = GetWindow()->GetEventDispatcher()->Subscribe<Engine::Event::WindowResizeEvent>(
		[this](const Engine::Event::WindowResizeEvent& event)
		{
			PositionUI();
		}
	);
}

void VictoryScene::UnsubscribeForEvents()
{
	GetWindow()->GetEventDispatcher()->Unsubscribe<Engine::Event::WindowResizeEvent>(m_windowResizeSubscription);
}
