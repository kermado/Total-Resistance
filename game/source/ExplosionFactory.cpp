#include "ExplosionFactory.hpp"

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>

#include "Behaviour/ParticleAffector.hpp"

ExplosionFactory::ExplosionFactory()
: IGameObjectFactory()
{
	// Nothing to do.
}

ExplosionFactory::~ExplosionFactory()
{
	// Nothing to do.
}

void ExplosionFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Scale multiple.
	const float scale = 30.0f;

	// Add a transform attribute.
	std::shared_ptr<Engine::Attribute::Transform> explosionTransform =
		gameObject->CreateAttribute<Engine::Attribute::Transform>();
	explosionTransform->SetScale(scale);
	explosionTransform->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	// Create child clouds.
	const unsigned int cloudCount = 20;
	for (unsigned int i = 0; i < cloudCount; ++i)
	{
		std::shared_ptr<Engine::GameObject> cloud = gameObject->CreateGameObject();

		// Add a transform attribute.
		std::shared_ptr<Engine::Attribute::Transform> cloudTransform =
			cloud->CreateAttribute<Engine::Attribute::Transform>();

		// Add a shader program attribute.
		std::shared_ptr<Engine::Attribute::ShaderProgram> shaderProgram =
			cloud->CreateAttribute<Engine::Attribute::ShaderProgram>(
				"resources/shaders/Phong.vert",
				"resources/shaders/Cloud.frag"
			);

		// Add a model attribute.
		cloud->CreateAttribute<Engine::Attribute::Model>("resources/models/explosion/Cloud.dae");

		// Add a particle affector behaviour.
		const float minInitialScale = 0.1f * scale;
		const float maxInitialScale = 0.1f * scale;
		const float initialScale = RandomInRange(minInitialScale, maxInitialScale);

		const float minFinalScale = 0.5f * scale;
		const float maxFinalScale = 1.0f * scale;
		const float finalScale = RandomInRange(minFinalScale, maxFinalScale);

		const float minTimeToLive = 1.5f;
		const float maxTimeToLive = 2.0f;
		const double timeToLive = static_cast<double>(RandomInRange(minTimeToLive, maxTimeToLive));

		const float x = RandomInRange(-0.002f, 0.002f) * scale;
		const float y = RandomInRange(0.0f, 0.003f) * scale;
		const float z = RandomInRange(-0.002f, 0.002f) * scale;

		cloud->CreateBehaviour<Behaviour::ParticleAffector>(
			cloudTransform,
			shaderProgram,
			timeToLive,
			glm::vec3(x, y, z) * std::min((maxFinalScale / finalScale), 4.0f),
			initialScale,
			finalScale
		);
	}
}

float ExplosionFactory::RandomInRange(float min, float max)
{
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float range = max - min;
	return min + (random * range);
}
