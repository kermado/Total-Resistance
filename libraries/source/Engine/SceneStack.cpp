#include <Engine/SceneStack.hpp>

namespace Engine
{
	SceneStack::SceneStack(std::shared_ptr<Window> window, std::shared_ptr<ResourceManager> resourceManager)
	: m_window(window)
	, m_resourceManager(resourceManager)
	, m_eventDispatcher(std::make_shared<EventDispatcher>())
	, m_stack()
	, m_factories()
	{
		// Subscribe for PushSceneEvents.
		m_eventDispatcher->Subscribe<Event::PushSceneEvent>(CALLBACK(SceneStack::HandlePushSceneEvent));

		// Subscribe for PopSceneEvents.
		m_eventDispatcher->Subscribe<Event::PopSceneEvent>(CALLBACK(SceneStack::HandlePopSceneEvent));
	}

	SceneStack::~SceneStack()
	{
		// Nothing to do.
	}

	void SceneStack::Update(double deltaTime)
	{
		m_eventDispatcher->Update();

		// Apply pending changes.
		ProcessPendingChanges();

		if (!m_stack.empty())
		{
			// Tell the active scene to update itself.
			std::shared_ptr<IScene> scene = m_stack.top();
			scene->Update(deltaTime);
		}
	}

	void SceneStack::DrawUI()
	{
		if (!m_stack.empty())
		{
			// Tell the scene at the top of the stack to draw its user
			// interface.
			std::shared_ptr<IScene> scene = m_stack.top();
			scene->OnDrawUI();
		}
	}

	void SceneStack::Render()
	{
		if (!m_stack.empty())
		{
			// Tell the scene at the top of the stack to render itself.
			std::shared_ptr<IScene> scene = m_stack.top();
			scene->Render();
		}
	}

	std::shared_ptr<IScene> SceneStack::Top()
	{
		assert(!m_stack.empty());
		return m_stack.top();
	}

	void SceneStack::HandlePushSceneEvent(const Event::PushSceneEvent& event)
	{
		Change change;
		change.action = Change::Action::Push;
		change.sceneName = event.GetSceneName();
		m_pendingChanges.push_back(change);
	}

	void SceneStack::HandlePopSceneEvent(const Event::PopSceneEvent& event)
	{
		Change change;
		change.action = Change::Action::Pop;
		m_pendingChanges.push_back(change);
	}

	bool SceneStack::Empty() const
	{
		return m_stack.empty();
	}

	void SceneStack::Clear()
	{
		Change change;
		change.action = Change::Action::Clear;
		m_pendingChanges.push_back(change);
	}

	void SceneStack::ProcessPendingChanges()
	{
		for (const Change& change : m_pendingChanges)
		{
			switch(change.action)
			{
				case Change::Action::Push:
				{
					if (!m_stack.empty())
					{
						std::shared_ptr<IScene> scene = m_stack.top();
						scene->OnSuspend();
					}

					assert(m_factories.find(change.sceneName) != m_factories.end());
					std::shared_ptr<IScene> createdScene = m_factories[change.sceneName]();
					m_stack.push(createdScene);
					createdScene->OnCreate();

					break;
				}

				case Change::Action::Pop:
				{
					assert(!m_stack.empty());

					// Pop the top scene off the stack.
					// The scene may be released from memory at this point.
					m_stack.pop();

					// Resume the next scene.
					if (!m_stack.empty())
					{
						std::shared_ptr<IScene> top = m_stack.top();
						top->OnResume();
					}

					break;
				}

				case Change::Action::Clear:
				{
					while (!m_stack.empty())
					{
						// Pop the top scene off the stack.
						// The scene may be released from memory at this point.
						m_stack.pop();
					}

					break;
				}
			}
		}

		m_pendingChanges.clear();
	}
}
