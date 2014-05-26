#include "Behaviour/SplashProjectile.hpp"

#include <Engine/Audio.hpp>

#include <Engine/Event/CreateGameObjectEvent.hpp>
#include <Engine/Event/DestroyGameObjectEvent.hpp>

#include "ExplosionFactory.hpp"
#include "Attribute/Tags.hpp"
#include "Event/InflictDamageEvent.hpp"

namespace Behaviour
{
	SplashProjectile::SplashProjectile(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
		const PlayingSurface& playingSurface,
		std::string tag,
		float damage)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_transformAttribute(transformAttribute)
	, m_playingSurface(playingSurface)
	, m_tag(tag)
	, m_damage(damage)
	, m_explosionFactory(std::make_shared<ExplosionFactory>())
	, m_inRange()
	, m_collisionSubscription(0)
	{
		// Subscribe to receive CollisionEvents.
		m_collisionSubscription = GetGameObjectEventDispatcher()->Subscribe<Engine::Event::CollisionEvent>(
			[this](const Engine::Event::CollisionEvent& event)
			{
				std::shared_ptr<Engine::GameObject> otherGameObject = event.GetOtherGameObject();
				if (!otherGameObject->IsDead() && otherGameObject->HasAttribute<Attribute::Tags>())
				{
					std::shared_ptr<Attribute::Tags> tagsAttribute =
						otherGameObject->GetAttribute<Attribute::Tags>();

					if (tagsAttribute->HasTag(m_tag))
					{
						m_inRange.push_back(otherGameObject);
					}
				}
			}
		);
	}

	SplashProjectile::~SplashProjectile()
	{
		// Unsubscribe for CollisionEvents.
		GetGameObjectEventDispatcher()->Unsubscribe<Engine::Event::CollisionEvent>(m_collisionSubscription);
	}

	void SplashProjectile::Update(double deltaTime)
	{
		const glm::vec3 position = m_transformAttribute->GetPosition();
		const glm::vec2 playingSurfaceHalfDimensions = m_playingSurface.GetDimensions() * 0.5f;

		// TODO: Add check for elevation above ground.
		if (position.x < - playingSurfaceHalfDimensions.x ||
			position.x > playingSurfaceHalfDimensions.x ||
			position.z < - playingSurfaceHalfDimensions.y ||
			position.z > playingSurfaceHalfDimensions.y ||
			position.y < 0.0f)
		{
			// If the projectile has hit the ground, then inflict damage on
			// the Game Objects within range.
			if (position.y < 0.0f)
			{
				for (std::shared_ptr<Engine::GameObject> gameObject : m_inRange)
				{
					gameObject->BroadcastEnqueue<Event::InflictDamageEvent>(m_damage);
				}

				// Play a large explosion sound.
				Engine::Audio::GetInstance().Play(GetResourceManager()->GetAudio("resources/audio/MissileExplosion.wav"));
			}

			GetSceneEventDispatcher()->Enqueue<Engine::Event::CreateGameObjectEvent>(
				m_explosionFactory,
				[this](std::shared_ptr<Engine::GameObject> explosion)
				{
					std::shared_ptr<Engine::Attribute::Transform> transform =
						explosion->GetAttribute<Engine::Attribute::Transform>();
					transform->SetPosition(m_transformAttribute->GetPosition());
				}
			);

			GetGameObjectEventDispatcher()->Enqueue<Engine::Event::DestroyGameObjectEvent>();
		}

		// Clear the list of Game Objects in range.
		m_inRange.clear();
	}
}
