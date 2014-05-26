#include <Engine/IScene.hpp>

namespace Engine
{
	IScene::IScene(std::shared_ptr<Window> window,
		std::shared_ptr<ResourceManager> resourceManager,
		std::shared_ptr<EventDispatcher> sceneStackEventDispatcher)
	: m_window(window)
	, m_resourceManager(resourceManager)
	, m_sceneStackEventDispatcher(sceneStackEventDispatcher)
	, m_eventDispatcher(std::make_shared<EventDispatcher>())
	, m_userInterfaceRenderer(window, resourceManager)
	{
		// Nothing to do.
	}

	IScene::~IScene()
	{
		// Nothing to do.
	}

	void IScene::Render()
	{
		// Nothing to do.
	}

	void IScene::Update(double deltaTime)
	{
		m_eventDispatcher->Update();
		OnUpdate(deltaTime);
	}

	void IScene::DrawShape(UI::IShape& shape,
		std::shared_ptr<ShaderProgram> shaderProgram)
	{
		m_userInterfaceRenderer.RenderShape(shape, shaderProgram);
	}

	std::shared_ptr<Window> IScene::GetWindow() const
	{
		return m_window;
	}

	std::shared_ptr<ResourceManager> IScene::GetResourceManager() const
	{
		return m_resourceManager;
	}

	std::shared_ptr<EventDispatcher> IScene::GetSceneStackEventDispatcher() const
	{
		return m_sceneStackEventDispatcher;
	}

	std::shared_ptr<EventDispatcher> IScene::GetEventDispatcher() const
	{
		return m_eventDispatcher;
	}
}
