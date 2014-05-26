#include "ScoutFactory.hpp"

#include <glm/glm.hpp>

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>
#include <Engine/Attribute/BoundingSphere.hpp>

#include "Behaviour/NavigateToCell.hpp"
#include "Attribute/Tags.hpp"
#include "Attribute/Health.hpp"
#include "Attribute/Resources.hpp"

ScoutFactory::ScoutFactory(std::shared_ptr<const PlayingSurface> playingSurface,
	Cell initialCell, Cell destinationCell, float healthMultiple)
: IGameObjectFactory()
, m_playingSurface(playingSurface)
, m_initialCell(initialCell)
, m_destinationCell(destinationCell)
, m_healthMultiple(healthMultiple)
{
	// Nothing to do.
}

ScoutFactory::~ScoutFactory()
{
	// Nothing to do.
}

void ScoutFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Add a transform attribute.
	std::shared_ptr<Engine::Attribute::Transform> transform =
		gameObject->CreateAttribute<Engine::Attribute::Transform>();
	transform->SetScale(25.0f);
	transform->SetRotation(glm::quat(glm::vec3(0.0f, -M_PI / 2.0f, 0.0f)));
	transform->SetPosition(m_playingSurface->GetPositionForCell(m_initialCell) - glm::vec3(50.0f, 0.0, 0.0f) + glm::vec3(0.0f, 20.0f, 0.0f));

	// Add a shader program.
	gameObject->CreateAttribute<Engine::Attribute::ShaderProgram>("resources/shaders/Phong.vert", "resources/shaders/Phong.frag");

	// Add a model attribute.
	gameObject->CreateAttribute<Engine::Attribute::Model>("resources/models/scout/Scout.dae");

	// Add a bounding sphere attribute.
	// Note that the sphere radius is multiplied by the transform's scale.
	gameObject->CreateAttribute<Engine::Attribute::BoundingSphere>(0.5f, "Scout");

	// Add a tags attribute.
	std::shared_ptr<Attribute::Tags> tags = gameObject->CreateAttribute<Attribute::Tags>();

	// Add some tags to identify the tank.
	tags->AddTag("Enemy");
	tags->AddTag("Scout");

	// Add a health attribute and set the initial health.
	gameObject->CreateAttribute<Attribute::Health>(2.0f * m_healthMultiple);

	// Add a resources attribute and set the metal carried by the Game Object.
	gameObject->CreateAttribute<Attribute::Resources>(100);

	// Add the "navigate to cell" behaviour.
	gameObject->CreateBehaviour<Behaviour::NavigateToCell>(transform, *m_playingSurface, m_destinationCell, 125.0f);
}
