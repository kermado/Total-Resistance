#include "RangeFactory.hpp"

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>

#include "Behaviour/ToggleRange.hpp"

RangeFactory::RangeFactory(float range)
: IGameObjectFactory()
, m_range(range)
{
	// Nothing to do.
}

RangeFactory::~RangeFactory()
{
	// Nothing to do.
}

void RangeFactory::CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const
{
	// Add a transform attribute to the Game Object.
	std::shared_ptr<Engine::Attribute::Transform> transform =
		gameObject->CreateAttribute<Engine::Attribute::Transform>();
	transform->SetLocalPosition(glm::vec3(0.0f, 0.01f, 0.0f));
	transform->SetLocalScale(m_range);

	// Add a shader program attribute to the Game Object.
	gameObject->CreateAttribute<Engine::Attribute::ShaderProgram>(
		"resources/shaders/Phong.vert",
		"resources/shaders/Phong.frag"
	);

	// Add a model attribute to the Game Object.
	std::shared_ptr<Engine::Attribute::Model> model =
		gameObject->CreateAttribute<Engine::Attribute::Model>("resources/models/range/Range.dae");
	model->SetVisible(false);

	// Add a toggle range behaviour to the Game Object.
	gameObject->CreateBehaviour<Behaviour::ToggleRange>(model);
}
