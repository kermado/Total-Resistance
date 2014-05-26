#include "HeavyBotFactory.hpp"

#include <glm/glm.hpp>

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>
#include <Engine/Attribute/BoundingSphere.hpp>

#include "Attribute/Tags.hpp"
#include "Attribute/Health.hpp"
#include "Attribute/Resources.hpp"
#include "Behaviour/NavigateToCell.hpp"
#include "Behaviour/Animate.hpp"

HeavyBotFactory::HeavyBotFactory(std::shared_ptr<const PlayingSurface> playingSurface, Cell initialCell, Cell destinationCell, float healthMultiple)
: IGameObjectFactory()
, m_playingSurface(playingSurface)
, m_initialCell(initialCell)
, m_destinationCell(destinationCell)
, m_healthMultiple(healthMultiple)
{
	// Nothing to do.
}

HeavyBotFactory::HeavyBotFactory(const HeavyBotFactory& other)
: IGameObjectFactory()
, m_playingSurface(other.m_playingSurface)
, m_initialCell(other.m_initialCell)
, m_destinationCell(other.m_destinationCell)
, m_healthMultiple(other.m_healthMultiple)
{
	// Nothing to do.
}

HeavyBotFactory::~HeavyBotFactory()
{
	// Nothing to do.
}

void HeavyBotFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Add a transform attribute.
	std::shared_ptr<Engine::Attribute::Transform> transform =
		gameObject->CreateAttribute<Engine::Attribute::Transform>();
	transform->SetScale(35.0f);
	transform->SetRotation(glm::quat(glm::vec3(0.0f, -M_PI / 2.0f, 0.0f)));
	transform->SetPosition(m_playingSurface->GetPositionForCell(m_initialCell) - glm::vec3(50.0f, 0.0, 0.0f));

	// Add a shader program attribute.
	gameObject->CreateAttribute<Engine::Attribute::ShaderProgram>("resources/shaders/Phong.vert", "resources/shaders/Phong.frag");

	// Add a model attribute.
	std::shared_ptr<Engine::Attribute::Model> model =
		gameObject->CreateAttribute<Engine::Attribute::Model>("resources/models/heavybot/AnimatedHeavyBot.dae");
	model->SetLoopAnimation(true);

	// Add a bounding sphere attribute.
	// Note that the sphere radius is multiplied by the transform's scale.
	gameObject->CreateAttribute<Engine::Attribute::BoundingSphere>(0.25f, "HeavyBot");

	// Add a tags attribute.
	std::shared_ptr<Attribute::Tags> tags = gameObject->CreateAttribute<Attribute::Tags>();

	// Add some tags to identify the Game Object.
	tags->AddTag("Enemy");
	tags->AddTag("HeavyBot");

	// Add a health attribute and set the initial health.
	gameObject->CreateAttribute<Attribute::Health>(20.0f * m_healthMultiple);

	// Add a resources attribute and set the metal carried by the Game Object.
	gameObject->CreateAttribute<Attribute::Resources>(250);

	// Add a "navigate to cell" behaviour.
	gameObject->CreateBehaviour<Behaviour::NavigateToCell>(
		transform,
		*m_playingSurface,
		m_destinationCell,
		75.0f // Movement speed (pixels/second).
	);

	// Add an animate behaviour.
	gameObject->CreateBehaviour<Behaviour::Animate>(model);
}
