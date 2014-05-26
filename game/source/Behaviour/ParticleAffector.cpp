#include "Behaviour/ParticleAffector.hpp"

#include <Engine/Event/DestroyGameObjectEvent.hpp>

#include <iostream>

namespace Behaviour
{
	ParticleAffector::ParticleAffector(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
		std::shared_ptr<Engine::Attribute::ShaderProgram> shaderProgramAttribute,
		double timeToLive,
		const glm::vec3& velocity,
		float startScale,
		float endScale)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_transformAttribute(transformAttribute)
	, m_shaderProgramAttribute(shaderProgramAttribute)
	, m_timeToLive(timeToLive)
	, m_velocity(velocity)
	, m_startScale(startScale)
	, m_endScale(endScale)
	, m_elapsedTime(0.0)
	{
		// Nothing to do.
	}

	ParticleAffector::~ParticleAffector()
	{
		// Nothing to do.
	}

	void ParticleAffector::Update(double deltaTime)
	{
		// Update the elapsed time.
		m_elapsedTime += deltaTime;

		// Request to destroy the game object if there is no time left to live.
		if (m_elapsedTime > m_timeToLive)
		{
			GetGameObjectEventDispatcher()->Enqueue<Engine::Event::DestroyGameObjectEvent>();
		}
		else
		{
			// Interpolate the scale using exponential easing out and update the
			// Game Object's transform attribute.
			m_transformAttribute->SetScale(EaseOutExponential(m_elapsedTime, m_startScale, m_endScale, m_timeToLive));

			// Translate the game object according to the velocity.
			m_transformAttribute->Translate(m_velocity);

			// Calculate the fraction of the particle's life that has been lived
			// and send set this value in the shader program.
			const double livedFraction = m_elapsedTime / m_timeToLive;
			m_shaderProgramAttribute->SetFloat("timeLeftFraction", 1.0f - static_cast<float>(livedFraction));
		}
	}

	float ParticleAffector::EaseOutExponential(double currentTime, float startValue, float endValue, double duration)
	{
		const float change = endValue - startValue;
		return change * (-std::pow(2, -15 * currentTime / duration) + 1) + startValue;
	}
}
