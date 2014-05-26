#include <Engine/Event/PushSceneEvent.hpp>

namespace Engine
{
	namespace Event
	{
		PushSceneEvent::PushSceneEvent(std::string sceneName)
		: m_sceneName(sceneName)
		{
			// Nothing to do.
		}

		PushSceneEvent::~PushSceneEvent()
		{
			// Nothing to do.
		}

		std::string PushSceneEvent::GetSceneName() const
		{
			return m_sceneName;
		}
	}
}
