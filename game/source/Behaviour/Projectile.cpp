#include "Behaviour/Projectile.hpp"

#include <Engine/Audio.hpp>

#include <Engine/Event/DestroyGameObjectEvent.hpp>

#include "Attribute/Tags.hpp"
#include "Event/InflictDamageEvent.hpp"

namespace Behaviour
{
	Projectile::Projectile(std::shared_ptr<Engine::Window> window,
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
	, m_collisionSubscription(0)
	{
		// Subscribe to receive CollisionEvents.
		m_collisionSubscription = GetGameObjectEventDispatcher()->Subscribe<Engine::Event::CollisionEvent>(CALLBACK(Projectile::HandleCollisionEvent));
	}

	Projectile::~Projectile()
	{
		// Unsubscribe for CollisionEvents.
		GetGameObjectEventDispatcher()->Unsubscribe<Engine::Event::CollisionEvent>(m_collisionSubscription);
	}

	void Projectile::Update(double deltaTime)
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
			GetGameObjectEventDispatcher()->Enqueue<Engine::Event::DestroyGameObjectEvent>();
		}
	}

	void Projectile::HandleCollisionEvent(const Engine::Event::CollisionEvent& event)
	{
		// Check whether the Game Object that we collided with has a tag that
		// indicates we can hit it and deal damage.
		std::shared_ptr<Engine::GameObject> otherGameObject = event.GetOtherGameObject();
		if (!otherGameObject->IsDead() && otherGameObject->HasAttribute<Attribute::Tags>())
		{
			std::shared_ptr<Attribute::Tags> tagsAttribute =
				otherGameObject->GetAttribute<Attribute::Tags>();

			if (tagsAttribute->HasTag(m_tag))
			{
				// Broadcast an InflictDamageEvent on the target Game Object.
				otherGameObject->BroadcastEnqueue<Event::InflictDamageEvent>(m_damage);

				// Request to destroy this Game Object.
				GetGameObjectEventDispatcher()->Enqueue<Engine::Event::DestroyGameObjectEvent>();

				// Play a small explosion sound.
				Engine::Audio::GetInstance().Play(GetResourceManager()->GetAudio("resources/audio/RocketExplosion.wav"));
			}
		}
	}
}
