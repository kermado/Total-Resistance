#include "MissileSiloFactory.hpp"

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>
#include <Engine/Attribute/BoundingSphere.hpp>

#include "MissileFactory.hpp"
#include "RangeFactory.hpp"

#include "Attribute/Tags.hpp"

#include "Behaviour/AcquireNearestTarget.hpp"
#include "Behaviour/FireMissileAtAcquiredTarget.hpp"
#include "Behaviour/ToggleRange.hpp"

MissileSiloFactory::MissileSiloFactory(std::shared_ptr<const PlayingSurface> playingSurface)
: IGameObjectFactory()
, m_playingSurface(playingSurface)
{
	// Nothing to do.
}

MissileSiloFactory::~MissileSiloFactory()
{
	// Nothing to do.
}

void MissileSiloFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Scale for the silo.
	const float scale = 20.0f;

	// Add a transform attribute.
	std::shared_ptr<Engine::Attribute::Transform> transform =
		gameObject->CreateAttribute<Engine::Attribute::Transform>();

	// Setup the transform.
	transform->SetScale(scale);

	// Add a shader program.
	gameObject->CreateAttribute<Engine::Attribute::ShaderProgram>("resources/shaders/Phong.vert", "resources/shaders/Phong.frag");

	// Add a model attribute.
	std::shared_ptr<Engine::Attribute::Model> model =
		gameObject->CreateAttribute<Engine::Attribute::Model>("resources/models/missilesilo/MissileSilo.dae");

	// Add a bounding sphere attribute.
	// This represents the range of the missile silo's radar.
	// Note that the sphere radius is multiplied by the transform's scale.
	gameObject->CreateAttribute<Engine::Attribute::BoundingSphere>(10.0f, "Radar");

	// Add a tags attribute.
	std::shared_ptr<Attribute::Tags> tags = gameObject->CreateAttribute<Attribute::Tags>();

	// Add a nearest target acquisition behaviour.
	gameObject->CreateBehaviour<Behaviour::AcquireNearestTarget>(transform, tags, "Enemy");

	// Add a fire missile at acquired target behaviour.
	gameObject->CreateBehaviour<Behaviour::FireMissileAtAcquiredTarget>(
		transform,
		model,
		tags,
		std::make_shared<MissileFactory>(m_playingSurface),
		7.0
	);

	// Add a child game object to the silo for the range display.
	RangeFactory factory(10.0f);
	gameObject->CreateGameObject(factory);
}
