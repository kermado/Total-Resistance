#include "LaserFactory.hpp"

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>

LaserFactory::LaserFactory()
: IGameObjectFactory()
{
	// Nothing to do.
}

LaserFactory::~LaserFactory()
{
	// Nothing to do.
}

void LaserFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Add a transform attribute.
	gameObject->CreateAttribute<Engine::Attribute::Transform>();

	// Add a shader program attribute.
	gameObject->CreateAttribute<Engine::Attribute::ShaderProgram>(
		"resources/shaders/Phong.vert",
		"resources/shaders/Phong.frag"
	);

	// Add a model attribute.
	gameObject->CreateAttribute<Engine::Attribute::Model>("resources/models/lasertower/Laser.dae");
}
