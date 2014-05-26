#ifndef LOADINGSCENE_H
#define	LOADINGSCENE_H

#include <Engine/IScene.hpp>
#include <Engine/Event/MouseButtonPressedEvent.hpp>
#include <Engine/UI/Rectangle.hpp>

/**
 * Loading scene.
 * Resources are loaded here.
 */
class LoadingScene : public Engine::IScene
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
	LoadingScene(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneStackEventDispatcher);

	/**
	 * Destructor.
	 */
	~LoadingScene();

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

	/**
	 * Handles MouseButtonPressedEvents.
	 *
	 * @param event Reference to the event.
	 */
	void HandleMouseButtonPressedEvent(const Engine::Event::MouseButtonPressedEvent& event);

protected:
	/**
	 * Positions the UI elements.
	 */
	void PositionUI();

	/**
	 * Subscribes the scene to receive various events.
	 */
	void SubscribeForEvents();

	/**
	 * Unsubscribes the scene to receive the events that were subscribed to in
	 * the SubscribeForEvents method.
	 */
	void UnsubscribeForEvents();

private:
	/**
	 * The loading bar UI element.
	 */
	Engine::UI::Rectangle m_loadingBar;

	/**
	 * The start game button UI element.
	 */
	Engine::UI::Rectangle m_startButton;

	/**
	 * The game title image.
	 */
	Engine::UI::Rectangle m_titleImage;

	/**
	 * The first scrolling stars background UI element.
	 */
	Engine::UI::Rectangle m_starsBackground1;

	/**
	 * The second scrolling stars background UI element.
	 */
	Engine::UI::Rectangle m_starsBackground2;

	/**
	 * The first scrolling nebula background UI element.
	 */
	Engine::UI::Rectangle m_nebulaBackground1;

	/**
	 * The second scrolling nebula background UI element.
	 */
	Engine::UI::Rectangle m_nebulaBackground2;

	/**
	 * Subscription identifier for the MouseButtonPressedEvent subscription.
	 */
	Engine::EventDispatcher::SubscriptionID m_mouseButtonPressedSubscription;

	/**
	 * Subscription identifier for the WindowResizeEvent subscription.
	 */
	Engine::EventDispatcher::SubscriptionID m_windowResizeSubscription;

	/**
	 * Number of resource loading requests made.
	 */
	unsigned int m_resourceLoadingRequestCount;

	/**
	 * Number of resource loading requests that have been completed.
	 */
	unsigned int m_resourceLoadingCompleteCount;

private:
	/**
	 * Width for the background UI elements.
	 */
	const float BACKGROUND_WIDTH = 1024.0f * 3.0f;

	/**
	 * Dimensions for the start game button UI element.
	 */
	const glm::vec2 START_BUTTON_DIMENSIONS = glm::vec2(174.0f, 52.0f);

	/**
	 * Dimensions for the title image UI element.
	 */
	const glm::vec2 TITLE_IMAGE_DIMENSIONS = glm::vec2(660.0f, 90.0f);
};

#endif
