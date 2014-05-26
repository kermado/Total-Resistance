#include "DefeatScene.hpp"

#include <Engine/Event/WindowResizeEvent.hpp>
#include <Engine/Event/PopSceneEvent.hpp>

DefeatScene::DefeatScene(std::shared_ptr<Engine::Window> window,
	std::shared_ptr<Engine::ResourceManager> resourceManager,
	std::shared_ptr<Engine::EventDispatcher> sceneStackEventDispatcher)
: Engine::IScene(window, resourceManager, sceneStackEventDispatcher)
, m_molotFont()
, m_defeatLabel()
, m_timeRemaining(5.0)
, m_windowResizeSubscription(0)
{
	// Nothing to do.
}

DefeatScene::~DefeatScene()
{
	UnsubscribeForEvents();
}

void DefeatScene::OnCreate()
{
	SubscribeForEvents();

	if (m_molotFont.LoadFromFile("resources/fonts/Molot.otf"))
	{
		m_defeatLabel.SetFont(m_molotFont);
		m_defeatLabel.SetCharacterSize(60);
		m_defeatLabel.SetString("Defeated");
	}

	PositionUI();
}

void DefeatScene::OnSuspend()
{
	UnsubscribeForEvents();
}

void DefeatScene::OnResume()
{
	SubscribeForEvents();
}

void DefeatScene::OnUpdate(double deltaTime)
{
	m_timeRemaining -= deltaTime;

	if (m_timeRemaining < 0)
	{
		// Pop this scene and the Game Scene off the scene stack.
		GetSceneStackEventDispatcher()->Enqueue<Engine::Event::PopSceneEvent>();
		GetSceneStackEventDispatcher()->Enqueue<Engine::Event::PopSceneEvent>();
	}
}

void DefeatScene::OnDrawUI()
{
	// Get the UI shader.
	std::shared_ptr<Engine::ShaderProgram> uiShader =
		GetResourceManager()->GetShaderProgram(
			"resources/shaders/UI.vert",
			"resources/shaders/UI.frag"
		);

	// Draw the victory label.
	DrawShape(m_defeatLabel, uiShader);
}

void DefeatScene::PositionUI()
{
	m_defeatLabel.SetPosition(GetWindow()->GetDimensions() * 0.5f - m_defeatLabel.GetDimensions() * 0.5f);
}

void DefeatScene::SubscribeForEvents()
{
	m_windowResizeSubscription = GetWindow()->GetEventDispatcher()->Subscribe<Engine::Event::WindowResizeEvent>(
		[this](const Engine::Event::WindowResizeEvent& event)
		{
			PositionUI();
		}
	);
}

void DefeatScene::UnsubscribeForEvents()
{
	GetWindow()->GetEventDispatcher()->Unsubscribe<Engine::Event::WindowResizeEvent>(m_windowResizeSubscription);
}
