#include "WallFactory.hpp"

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>

WallFactory::WallFactory()
{
	// Nothing to do.
}

WallFactory::~WallFactory()
{
	// Nothing to do.
}

void WallFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Add a transform attribute.
	std::shared_ptr<Engine::Attribute::Transform> transform =
		gameObject->CreateAttribute<Engine::Attribute::Transform>();
	transform->SetScale(20.0f);

	// Add a shader program attribute.
	gameObject->CreateAttribute<Engine::Attribute::ShaderProgram>("resources/shaders/Phong.vert", "resources/shaders/Phong.frag");

	// Add a model attribute.
	gameObject->CreateAttribute<Engine::Attribute::Model>("resources/models/wall/Wall.dae");
}
