#ifndef MODELATTRIBUTE_H
#define	MODELATTRIBUTE_H

#include <string>
#include <memory>

#include <Engine/Attribute/IAttribute.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>

namespace Engine
{
	namespace Attribute
	{
		class Model : public IAttribute
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param window Shared pointer to the window.
			 * @param resourceManager Shared pointer to the resource manager.
			 * @param sceneEventDispatcher Shared pointer to the scene's Event
			 * Dispatcher.
			 * @param gameObjectEventDispatcher Shared pointer to the Game
			 * Object's Event Dispatcher.
			 * @param gameObject Weak pointer to the Game Object that the
			 * attribute is attached to.
			 * @param filepath Path to the model.
			 */
			Model(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject,
				std::string filepath);

			/**
			 * Destructor.
			 */
			virtual ~Model();

			/**
			 * Returns the path to the model.
			 *
			 * @return Path to the model.
			 */
			std::string GetPath() const;

			/**
			 * Returns true if the model should be rendered.
			 *
			 * @return True if the model is visible and should be rendered.
			 */
			bool GetVisible() const;

			/**
			 * Sets whether the model should be rendered.
			 *
			 * @param visible visible Render the model?
			 */
			void SetVisible(bool visible);

			/**
			 * Returns the current animation time.
			 *
			 * @return Current animation time (in seconds).
			 */
			double GetCurrentAnimationTime() const;

			/**
			 * Restarts the animation from the beginning.
			 */
			void RestartAnimation();

			/**
			 * Advances the animation by the specified time.
			 *
			 * @param deltaTime Time to advance the animation by (in seconds).
			 */
			void AdvanceAnimation(double deltaTime);

			/**
			 * Returns the duration of the animation.
			 *
			 * @return Animation duration.
			 */
			double GetAnimationDuration() const;

			/**
			 * Returns true if the animation is set to loop.
			 *
			 * @retur True if animation loops.
			 */
			bool GetLoopAnimation() const;

			/**
			 * Sets whether the animation should loop.
			 *
			 * @param loop Loop animation?
			 */
			void SetLoopAnimation(bool loop);

		private:
			/**
			 * Path to the model.
			 */
			std::string m_filepath;

			/**
			 * Whether the model should be rendered.
			 */
			bool m_visible;

			/**
			 * Current animation time.
			 */
			double m_currentAnimationTime;

			/**
			 * Whether to loop the animation.
			 */
			bool m_loop;
		};
	}
}

#endif
