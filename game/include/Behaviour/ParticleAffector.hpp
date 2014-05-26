#ifndef PARTICLEAFFECTOR_H
#define PARTICLEAFFECTOR_H

#include <glm/glm.hpp>

#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>

namespace Behaviour
{
	class ParticleAffector : public Engine::Behaviour::IBehaviour
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param window Shared pointer to the window.
		 * @param resourceManager Shared pointer to the resource manager.
		 * @param sceneEventDispatcher Shared pointer to the scene's event
		 * dispatcher.
		 * @param gameObjectEventDispatcher Shared pointer to the game
		 * object's event dispatcher.
		 * @param gameObject Weak pointer to the Game Object that the
		 * behaviour is attached to.
		 * @param transformAttribute Shared pointer to the game object's
		 * transform attribute.
		 * @param shaderProgramAttribute Shared pointer to the game object's
		 * shader program attribute.
		 * @param timeToLive Time for the game object to live (in seconds).
		 * @param velocity The game object's velocity.
		 * @param startScale The initial scale for the game object.
		 * @param endScale The final scale for the game object.
		 */
		ParticleAffector(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
			std::shared_ptr<Engine::Attribute::ShaderProgram> shaderProgramAttribute,
			double timeToLive,
			const glm::vec3& velocity,
			float startScale,
			float endScale);

		/**
		 * Destructor.
		 */
		~ParticleAffector();

		/**
		 * Transforms the game object.
		 */
		virtual void Update(double deltaTime);

	private:
		/**
		 * Returns the interpolated value from the starting value to the ending
		 * value by the current time and given total duration using exponential
		 * easing out.
		 *
		 * @param currentTime The current time.
		 * @param startValue The starting value.
		 * @param endValue The ending value.
		 * @param duration The total duration.
		 * @return Interpolated value using exponential easing out.
		 */
		static float EaseOutExponential(double currentTime, float startValue, float endValue, double duration);

	private:
		/**
		 * Shared pointer to the game object's transform attribute.
		 */
		std::shared_ptr<Engine::Attribute::Transform> m_transformAttribute;

		/**
		 * Shared pointer to the game object's shader program attribute.
		 */
		std::shared_ptr<Engine::Attribute::ShaderProgram> m_shaderProgramAttribute;

		/**
		 * The amount of time that the game object should live for (in seconds).
		 */
		double m_timeToLive;

		/**
		 * The game object's velocity.
		 */
		glm::vec3 m_velocity;

		/**
		 * The inital scale for the game object.
		 */
		float m_startScale;

		/**
		 * The final scale for the game object.
		 */
		float m_endScale;

		/**
		 * The amount of elapsed time (in seconds).
		 */
		double m_elapsedTime;
	};
}

#endif
