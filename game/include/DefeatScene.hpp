#ifndef DEFEATSCENE_H
#define DEFEATSCENE_H

#include <Engine/IScene.hpp>
#include <Engine/EventDispatcher.hpp>

#include <Engine/UI/Font.hpp>
#include <Engine/UI/Text.hpp>

class DefeatScene : public Engine::IScene
{
public:
	/**
	 * Constructor.
	 *
	 * @param window Shared pointer to the window.
	 * @param resourceManager Shared pointer to the resource manager.
	 * @param sceneStackEventDispatcher Shared pointer to the scene stack's
	 * event dispatcher.
	 */
	DefeatScene(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneStackEventDispatcher);

	/**
	 * Destructor.
	 */
	virtual ~DefeatScene();

	/**
	 * Scene was created.
	 */
	virtual void OnCreate();

	/**
	 * Scene was suspended.
	 */
	virtual void OnSuspend();

	/**
	 * Scene was resumed.
	 */
	virtual void OnResume();

	/**
	 * Updates the scene.
	 *
	 * @param deltaTime Time since last call (in seconds).
	 */
	virtual void OnUpdate(double deltaTime);

	/**
	 * Draws the user interface.
	 */
	virtual void OnDrawUI();

private:
	/**
	 * Repositions UI elements.
	 */
	void PositionUI();

	/**
	 * Subscribes to receive events.
	 */
	void SubscribeForEvents();

	/**
	 * Unsubscribes to receive events.
	 */
	void UnsubscribeForEvents();

private:
	/**
	 * Molot font.
	 */
	Engine::UI::Font m_molotFont;

	/**
	 * The defeat label UI element.
	 */
	Engine::UI::Text m_defeatLabel;

	/**
	 * The number of seconds before returning to the Loading Scene.
	 */
	double m_timeRemaining;

	/**
	 * Subscription for window resize events.
	 */
	Engine::EventDispatcher::SubscriptionID m_windowResizeSubscription;
};

#endif
