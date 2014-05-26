#include "ExhaustFactory.hpp"

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>

#include "Behaviour/ParticleAffector.hpp"

ExhaustFactory::ExhaustFactory()
: IGameObjectFactory()
{
	// Nothing to do.
}

ExhaustFactory::~ExhaustFactory()
{
	// Nothing to do.
}

void ExhaustFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Scale multiple.
	const float scale = 15.0f;

	// Add a transform attribute.
	std::shared_ptr<Engine::Attribute::Transform> transform =
		gameObject->CreateAttribute<Engine::Attribute::Transform>();
	transform->SetScale(scale);
	transform->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	// Add a shader program attribute.
	std::shared_ptr<Engine::Attribute::ShaderProgram> shaderProgram =
		gameObject->CreateAttribute<Engine::Attribute::ShaderProgram>(
			"resources/shaders/Phong.vert",
			"resources/shaders/Cloud.frag"
		);

	// Add a model attribute.
	gameObject->CreateAttribute<Engine::Attribute::Model>("resources/models/explosion/Cloud.dae");

	// Add a particle affector behaviour.
	gameObject->CreateBehaviour<Behaviour::ParticleAffector>(
		transform,
		shaderProgram,
		0.25, // Time to live
		glm::vec3(RandomInRange(-0.25, 0.25), 0.0f, RandomInRange(-0.25, 0.25)), // Velocity
		0.1f * scale, // Initial scale
		scale * RandomInRange(0.9f, 1.1f) // Final scale
	);
}

float ExhaustFactory::RandomInRange(float min, float max)
{
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float range = max - min;
	return min + (random * range);
}
