#include <Engine/Attribute/Model.hpp>

namespace Engine
{
	namespace Attribute
	{
		Model::Model(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject,
				std::string filepath)
		: IAttribute(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
		, m_filepath(filepath)
		, m_visible(true)
		, m_currentAnimationTime(0)
		, m_loop(false)
		{
			// Nothing to do.
		}

		Model::~Model()
		{
			// Nothing to do.
		}

		std::string Model::GetPath() const
		{
			return m_filepath;
		}

		bool Model::GetVisible() const
		{
			return m_visible;
		}

		void Model::SetVisible(bool visible)
		{
			m_visible = visible;
		}

		double Model::GetCurrentAnimationTime() const
		{
			return m_currentAnimationTime;
		}

		void Model::RestartAnimation()
		{
			m_currentAnimationTime = 0.0;
		}

		void Model::AdvanceAnimation(double deltaTime)
		{
			assert(deltaTime > 0);
			m_currentAnimationTime += deltaTime;

			// Adjust for looping.
			if (m_loop && m_currentAnimationTime > GetAnimationDuration())
			{
				m_currentAnimationTime = 0.0;
			}
		}

		double Model::GetAnimationDuration() const
		{
			std::shared_ptr<Engine::Model> modelResource = GetResourceManager()->GetModel(m_filepath);
			return modelResource->GetAnimationDuration();
		}

		bool Model::GetLoopAnimation() const
		{
			return m_loop;
		}

		void Model::SetLoopAnimation(bool loop)
		{
			m_loop = loop;
		}
	}
}
