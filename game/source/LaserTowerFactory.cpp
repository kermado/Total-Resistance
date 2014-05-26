#include "LaserTowerFactory.hpp"

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>
#include <Engine/Attribute/BoundingSphere.hpp>

#include "RocketFactory.hpp"
#include "RangeFactory.hpp"

#include "Attribute/Tags.hpp"

#include "Behaviour/AcquireNearestTarget.hpp"
#include "Behaviour/FaceAcquiredTarget.hpp"
#include "Behaviour/FireLaserAtAcquiredTarget.hpp"
#include "Behaviour/ToggleRange.hpp"

LaserTowerFactory::LaserTowerFactory(std::shared_ptr<const PlayingSurface> playingSurface)
: IGameObjectFactory()
, m_playingSurface(playingSurface)
{
	// Nothing to do.
}

LaserTowerFactory::~LaserTowerFactory()
{
	// Nothing to do.
}

void LaserTowerFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Add a transform attribute to the base.
	std::shared_ptr<Engine::Attribute::Transform> baseTransform =
		gameObject->CreateAttribute<Engine::Attribute::Transform>();
	baseTransform->SetScale(20.0f);

	// Add a shader program to the base.
	gameObject->CreateAttribute<Engine::Attribute::ShaderProgram>(
		"resources/shaders/Phong.vert",
		"resources/shaders/Phong.frag"
	);

	// Add a model attribute to the base.
	gameObject->CreateAttribute<Engine::Attribute::Model>("resources/models/lasertower/LaserTowerBase.dae");

	// Add a child game object to the base for the turret.
	std::shared_ptr<Engine::GameObject> turret = gameObject->CreateGameObject();

	// Add a transform attribute to the turret.
	std::shared_ptr<Engine::Attribute::Transform> turretTransform =
		turret->CreateAttribute<Engine::Attribute::Transform>();
	turretTransform->SetLocalPosition(glm::vec3(0.0f, 25.0f, 0.0f));

	// Add a shader program attribute to the turret.
	turret->CreateAttribute<Engine::Attribute::ShaderProgram>(
		"resources/shaders/Phong.vert",
		"resources/shaders/Phong.frag"
	);

	// Add a model attribute to the turret.
	turret->CreateAttribute<Engine::Attribute::Model>("resources/models/lasertower/LaserTowerTurret.dae");

	// Add a bounding sphere attribute to the turret.
	// This represents the range of the laser tower's radar.
	// Note that the sphere radius is multiplied by the transform's scale.
	turret->CreateAttribute<Engine::Attribute::BoundingSphere>(4.0f, "Radar");

	// Add a tags attribute to the turret.
	std::shared_ptr<Attribute::Tags> turretTags = turret->CreateAttribute<Attribute::Tags>();

	// Add a nearest target acquisition behaviour to the turret.
	turret->CreateBehaviour<Behaviour::AcquireNearestTarget>(turretTransform, turretTags, "Enemy");

	// Add a face target behaviour to the turret.
	turret->CreateBehaviour<Behaviour::FaceAcquiredTarget>(
		turretTransform,
		0.75f * 2.0f * M_PI // Rotation speed (radians per second)
	);

	// Add a fire laser at acquired target behaviour to the turret.
	// Add a fire rockets at acquired targe behaviour to the turret.
	turret->CreateBehaviour<Behaviour::FireLaserAtAcquiredTarget>(turretTransform);

	// Add a child game object to the base for the range display.
	RangeFactory factory(4.0f);
	gameObject->CreateGameObject(factory);
}
