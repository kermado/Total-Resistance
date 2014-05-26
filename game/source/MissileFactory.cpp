#include "MissileFactory.hpp"

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>
#include <Engine/Attribute/BoundingSphere.hpp>

#include "Attribute/Tags.hpp"
#include "Behaviour/MissileController.hpp"
#include "Behaviour/SplashProjectile.hpp"

MissileFactory::MissileFactory(std::shared_ptr<const PlayingSurface> playingSurface)
: IGameObjectFactory()
, m_playingSurface(playingSurface)
{
	// Nothing to do.
}

MissileFactory::~MissileFactory()
{
	// Nothing to do.
}

void MissileFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Scale for the missile.
	const float scale = 10.0f;

	// Add a transform attribute.
	std::shared_ptr<Engine::Attribute::Transform> transform =
		gameObject->CreateAttribute<Engine::Attribute::Transform>();
	transform->SetScale(scale);

	// Add a shader program.
	gameObject->CreateAttribute<Engine::Attribute::ShaderProgram>(
		"resources/shaders/Phong.vert",
		"resources/shaders/Phong.frag"
	);

	// Add a model attribute.
	gameObject->CreateAttribute<Engine::Attribute::Model>("resources/models/missilesilo/Missile.dae");

	// Add a bounding sphere attribute.
	gameObject->CreateAttribute<Engine::Attribute::BoundingSphere>(10.0f, "Missile");

	// Add a tags attribute.
	std::shared_ptr<Attribute::Tags> tags = gameObject->CreateAttribute<Attribute::Tags>();

	// Add some tags to identify the game object.
	tags->AddTag("Projectile");
	tags->AddTag("Missile");

	// Add a splash projectile behaviour.
	gameObject->CreateBehaviour<Behaviour::SplashProjectile>(
		transform,
		*m_playingSurface,
		"Enemy",
		100.0f
	);

	// Add a missile controller behaviour.
	gameObject->CreateBehaviour<Behaviour::MissileController>(
		transform,
		240.0f,
		700.0f
	);
}
