#include <Engine/IGameScene.hpp>

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/OrthographicCamera.hpp>

namespace Engine
{
	IGameScene::IGameScene(std::shared_ptr<Window> window, std::shared_ptr<ResourceManager> resourceManager,
		std::shared_ptr<EventDispatcher> sceneStackEventDispatcher)
	: IScene(window, resourceManager, sceneStackEventDispatcher)
	, m_gameObjectRenderer(resourceManager)
	, m_gameObjectCollider()
	, m_gameObjects()
	, m_cameraGameObject(nullptr)
	, m_createGameObjectSubscription(0)
	{
		// Subscribe for create game object events.
		m_createGameObjectSubscription = GetEventDispatcher()->Subscribe<Event::CreateGameObjectEvent>(
			[this] (const Event::CreateGameObjectEvent& event) {
				std::shared_ptr<const IGameObjectFactory> factory = event.GetFactory();
				if (factory)
				{
					event.ExecuteCallback(CreateGameObject(*factory));
				}
				else
				{
					event.ExecuteCallback(CreateGameObject());
				}
			}
		);
	}

	IGameScene::~IGameScene()
	{
		// Unsubscribe for create game object events.
		GetEventDispatcher()->Unsubscribe<Event::CreateGameObjectEvent>(m_createGameObjectSubscription);
	}

	void IGameScene::Update(double deltaTime)
	{
		GetEventDispatcher()->Update();

		// Perform scene update logic.
		OnUpdate(deltaTime);

		// Process collisions.
		m_gameObjectCollider.Process(*this);

		// Update all living Game Objects and remove from the scene those that
		// have been marked as dead.
		auto iter = m_gameObjects.begin();
		while (iter != m_gameObjects.end())
		{
			// Get the Game Object.
			std::shared_ptr<GameObject> gameObject = iter->second;

			// Remove the Game Object from the scene if it has been marked
			// as dead.
			if (gameObject->IsDead())
			{
				iter = m_gameObjects.erase(iter);
			}
			else
			{
				gameObject->Update(deltaTime);
				++iter;
			}
		}
	}

	void IGameScene::Render()
	{
		IScene::Render();
		m_gameObjectRenderer.Render(m_gameObjects, m_cameraGameObject);
	}

	std::shared_ptr<GameObject> IGameScene::CreateGameObject(const IGameObjectFactory& factory)
	{
		// Create a new Game Object.
		std::shared_ptr<GameObject> gameObject = CreateGameObject();

		// Use the factory to decorate a new Game Object.
		factory.CreateGameObject(gameObject);

		// Return a pointer to the decorated Game Object.
		return gameObject;
	}

	std::shared_ptr<GameObject> IGameScene::CreateGameObject()
	{
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(
			GetWindow(),
			GetResourceManager(),
			GetEventDispatcher()
		);

		m_gameObjects[gameObject->GetId()] = gameObject;
		return gameObject;
	}

	std::shared_ptr<GameObject> IGameScene::GetCamera() const
	{
		return m_cameraGameObject;
	}

	void IGameScene::SetCamera(std::shared_ptr<GameObject> gameObject)
	{
		// Check that the specified Game Object has the required attributes.
		assert(!gameObject->IsDead() &&
			gameObject->HasAttribute<Attribute::OrthographicCamera>() &&
			gameObject->HasAttribute<Attribute::Transform>());

		m_cameraGameObject = gameObject;
	}

	unsigned int IGameScene::GetGameObjectCount() const
	{
		return m_gameObjects.size();
	}

	std::shared_ptr<GameObject> IGameScene::GetGameObject(unsigned int index) const
	{
		assert(index < m_gameObjects.size());
		auto iter = m_gameObjects.begin();
		std::advance(iter, index);
		return iter->second;
	}

	Ray IGameScene::ScreenPointToRay(const glm::vec2& screenPoint) const
	{
		// The scene have a valid active camera in order to calculate the ray.
		assert(m_cameraGameObject &&
			m_cameraGameObject->HasAttribute<Attribute::Transform>() &&
			m_cameraGameObject->HasAttribute<Attribute::OrthographicCamera>());

		// Get the camera's transform and orthographic projection attributes.
		std::shared_ptr<Attribute::Transform> cameraTransformAttribute =
			m_cameraGameObject->GetAttribute<Attribute::Transform>();
		std::shared_ptr<Attribute::OrthographicCamera> cameraOrthographicAttribute =
			m_cameraGameObject->GetAttribute<Attribute::OrthographicCamera>();

		// Convert mouse viewport coordinates to 3D Normalised Device Coordinates (NDC)
		// where the range is [-1:1, -1:1, -1:1].
		float x = (2.0f * screenPoint.x) / GetWindow()->GetWidth() - 1.0f;
		float y = 1.0f - (2.0f * screenPoint.y) / GetWindow()->GetHeight();
		const glm::vec2 mouseNdcCoord(x, y);

		// Convert the NDC coordinates into Homogeneous Clip coordinates.
		// We want our ray to point forwards, which is in the direction of the
		// negative z-axis in OpenGL.
		const glm::vec4 mouseClipCoord(mouseNdcCoord, -1.0f, 1.0f);

		// Convert the Homogeneous Clip coordinates into Eye coordinates.
		glm::vec4 mouseEyeCoord(glm::inverse(cameraOrthographicAttribute->GetProjectionMatrix()) * mouseClipCoord);
		mouseEyeCoord.z = -1.0f;
		mouseEyeCoord.w = 1.0f;

		// Convert the Eye coordinates to World coordinates.
		// Note: The view matrix is the inverse of the camera's transformation
		// matrix. Therefore, the inverse of the view matrix is exactly the
		// camera's transformation matrix.
		const glm::mat4& inverseViewMatrix = cameraTransformAttribute->GetTransformationMatrix();
		const glm::vec3 mouseWorldCoord(inverseViewMatrix * mouseEyeCoord);

		// Return a ray from the mouse point (world coordinates) in the
		// direction of the camera.
		return Ray(
			mouseWorldCoord, // origin
			cameraTransformAttribute->GetForward() // direction
		);
	}
}
