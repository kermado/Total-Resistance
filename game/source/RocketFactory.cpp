#include "RocketFactory.hpp"

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>
#include <Engine/Attribute/BoundingSphere.hpp>

#include "Attribute/Tags.hpp"
#include "Behaviour/MoveForwards.hpp"
#include "Behaviour/Projectile.hpp"

RocketFactory::RocketFactory(std::shared_ptr<const PlayingSurface> playingSurface)
: IGameObjectFactory()
, m_playingSurface(playingSurface)
{
	// Nothing to do.
}

RocketFactory::~RocketFactory()
{
	// Nothing to do.
}

void RocketFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Add a transform attribute to the rocket.
	std::shared_ptr<Engine::Attribute::Transform> transform =
		gameObject->CreateAttribute<Engine::Attribute::Transform>();
	transform->SetScale(10.0f);

	// Add a shader program to the rocket.
	gameObject->CreateAttribute<Engine::Attribute::ShaderProgram>(
		"resources/shaders/Phong.vert",
		"resources/shaders/Phong.frag"
	);

	// Add a model attribute to the rocket.
	gameObject->CreateAttribute<Engine::Attribute::Model>("resources/models/rocketlauncher/Rocket.dae");

	// Add a bounding sphere attribute to the rocket.
	gameObject->CreateAttribute<Engine::Attribute::BoundingSphere>(1.0f, "Projectile");

	// Add a tags attribute to the rocket.
	std::shared_ptr<Attribute::Tags> tags = gameObject->CreateAttribute<Attribute::Tags>();

	// Add tags to the rocket.
	tags->AddTag("Projectile");
	tags->AddTag("Rocket");

	// Add a move forwards behaviour to the rocket.
	gameObject->CreateBehaviour<Behaviour::MoveForwards>(transform, 300.0f);

	// Add a projectile behaviour to the rocket.
	gameObject->CreateBehaviour<Behaviour::Projectile>(transform, *m_playingSurface, "Enemy", 1.0f);
}
