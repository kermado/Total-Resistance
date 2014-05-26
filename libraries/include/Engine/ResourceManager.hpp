#ifndef RESOURCEMANAGER_H
#define	RESOURCEMANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <thread>

#include <Engine/NonCopyable.hpp>
#include <Engine/Window.hpp>
#include <Engine/ThreadEventReceiver.hpp>

#include <Engine/ShaderProgram.hpp>
#include <Engine/Model.hpp>
#include <Engine/Texture.hpp>
#include <Engine/IAudioSource.hpp>

#include <Engine/Event/LoadModelResourceEvent.hpp>
#include <Engine/Event/LoadShaderProgramResourceEvent.hpp>
#include <Engine/Event/LoadTextureResourceEvent.hpp>
#include <Engine/Event/LoadAudioResourceEvent.hpp>

namespace Engine
{
	/**
	 * Performs the actual loading.
	 * The ResourceLoader::Run() method should be the entry point
	 * for the loading thread.
	 */
	class ResourceLoader : private NonCopyable
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param gameThreadReceiver Shared pointer to the main thread event
		 * receiver.
		 * @param loadingWindow Shared pointer to the window with the OpenGL
		 * context used for loading resources.
		 */
		ResourceLoader(std::shared_ptr<ThreadEventReceiver> gameThreadReceiver,
			std::shared_ptr<Window> loadingWindow);

		/**
		 * Destructor.
		 */
		~ResourceLoader();

		/**
		 * Runs the resource loader. This is executed from the
		 * loading thread.
		 */
		void Run();

		/**
		 * Returns a reference to the receiver for the loader
		 * thread.
		 */
		ThreadEventReceiver& GetReceiver();

		/**
		 * Handles LoadModelResourceEvents by loading the requested
		 * model.
		 *
		 * @note This method is executed from the loading thread.
		 *
		 * @param event Reference to the event.
		 */
		void HandleLoadModelResourceEvent(const Event::LoadModelResourceEvent& event);

		/**
		 * Handles LoadShaderProgramResourceEvents by loading the requested
		 * shader program.
		 *
		 * @note This method is executed from the loading thread.
		 *
		 * @param event Reference to the event.
		 */
		void HandleLoadShaderProgramResourceEvent(const Event::LoadShaderProgramResourceEvent& event);

		/**
		 * Handles LoadTextureResourceEvents by loading the requested
		 * texture.
		 *
		 * @note This method is executed from the loading thread.
		 *
		 * @param event Reference to the event.
		 */
		void HandleLoadTextureResourceEvent(const Event::LoadTextureResourceEvent& event);

		/**
		 * Handles LoadAudioResourceEvents by loading the requested
		 * audio source.
		 *
		 * @note This method is executed from the loading thread.
		 *
		 * @param event Reference to the event.
		 */
		void HandleLoadAudioResourceEvent(const Event::LoadAudioResourceEvent& event);

		/**
		 * Handles StopThreadEvents by setting a flag to return out of
		 * the loading thread entry method.
		 *
		 * @param event Reference to the event.
		 */
		void HandleStopThreadEvent(const Event::StopThreadEvent& event);

	private:
		/**
		 * Should the loading thread be terminated?
		 */
		bool m_terminateLoadingThread;

		/**
		 * Event receiver for the loading thread.
		 */
		ThreadEventReceiver m_loadingThreadEventReceiver;

		/**
		 * Reference to the game thread event receiver.
		 */
		std::shared_ptr<ThreadEventReceiver> m_gameThreadEventReceiver;

		/**
		 * Window with context for loading.
		 */
		std::shared_ptr<Window> m_window;

		/**
		 * Subscription identifier for the LoadModelResourceEvent subscription.
		 */
		ThreadEventReceiver::SubscriptionID m_loadModelSubscription;

		/**
		 * Subscription identifier for the LoadShaderProgramResourceEvent
		 * subscription.
		 */
		ThreadEventReceiver::SubscriptionID m_loadShaderProgramSubscription;

		/**
		 * Subscription identifier for the LoadTextureResourceEvent
		 * subscription.
		 */
		ThreadEventReceiver::SubscriptionID m_loadTextureSubscription;

		/**
		 * Subscription identifier for the LoadAudioResourceEvent subscription.
		 */
		ThreadEventReceiver::SubscriptionID m_loadAudioSubscription;

		/**
		 * Subscription identifier for the StopThreadEvent subscription.
		 */
		ThreadEventReceiver::SubscriptionID m_stopThreadSubscription;
	};

	/**
	 * Provides an interface for loading and accessing resources.
	 * This class also holds pointers to any loaded resources.
	 * This class starts the loading thread.
	 *
	 * @note The actual loading is performed by the ResourceLoader
	 * class.
	 */
	class ResourceManager : private NonCopyable
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param Shared pointer to the window to use for the loading thread.
		 */
		ResourceManager(std::shared_ptr<Window> loadingWindow);

		/**
		 * Destructor.
		 */
		~ResourceManager();

		/**
		 * Processes events in game thread event receiver.
		 */
		void Update();

		/**
		 * Loads a shader program given paths to the vertex and fragment shader
		 * source files.
		 *
		 * @param vertexShaderFilepath Path to the vertex shader.
		 * @param fragmentShaderFilepath Path to the fragment shader.
		 * @param callback Callback function.
		 */
		void LoadShaderProgram(std::string vertexShaderFilepath,
			std::string fragmentShaderFilepath,
			std::function<void(const Event::ResourceLoadedEvent<ShaderProgram>&)> callback);

		/**
		 * Returns a shared pointer to the shader program specified by the
		 * provided vertex and fragment shader file paths.
		 *
		 * @param vertexShaderFilepath Path to the vertex shader.
		 * @param fragmentShaderFilepath Path to the fragment shader.
		 * @return Shared pointer to the shader program.
		 */
		std::shared_ptr<ShaderProgram> GetShaderProgram(std::string vertexShaderFilepath,
			std::string fragmentShaderFilepath);

		/**
		 * Loads a 3D model from the specified file.
		 *
		 * @param filepath Path to the model file.
		 * @param onCompleteCallback Callback function.
		 */
		void LoadModel(std::string filepath,
			std::function<void(const Event::ResourceLoadedEvent<Model>&)> callback);

		/**
		 * Returns a shared pointer to the model specified by the provided file
		 * path.
		 *
		 * @param filepath Path to the model file.
		 * @return Shared pointer to the model.
		 */
		std::shared_ptr<Model> GetModel(std::string filepath);

		/**
		 * Loads a texture from the specified image file.
		 *
		 * @param filepath Path to the image file.
		 * @param onCompleteCallback Callback function.
		 */
		void LoadTexture(std::string filepath,
			std::function<void(const Event::ResourceLoadedEvent<Texture>&)> callback);

		/**
		 * Returns a shared pointer to the texture specified by the provided
		 * image file path.
		 *
		 * @param filepath Path to the image file.
		 * @return Shared pointer to the texture.
		 */
		std::shared_ptr<Texture> GetTexture(std::string filepath);

		/**
		 * Loads a audio source from the specified audio file.
		 *
		 * @param filepath Path to the audio file.
		 * @param onCompleteCallback Callback function.
		 */
		void LoadAudio(std::string filepath,
			std::function<void(const Event::ResourceLoadedEvent<IAudioSource>&)> callback);

		/**
		 * Returns a shared pointer to the audio source specified by the
		 * provided audio file path.
		 *
		 * @param filepath Path to the audio file.
		 * @return Shared pointer to the audio source.
		 */
		std::shared_ptr<IAudioSource> GetAudio(std::string filepath);

		/**
		 * Handles ResourceLoadedEvents for models by executing the
		 * callback function.
		 *
		 * @note This method is executed from the main game thread.
		 *
		 * @param event Reference to the event.
		 */
		void HandleModelResourceLoadedEvent(const Event::ResourceLoadedEvent<Model>& event);

		/**
		 * Handles ResourceLoadedEvents for shader programs by
		 * executing the callback function.
		 *
		 * @note This method is executed from the main game thread.
		 *
		 * @param event Reference to the event.
		 */
		void HandleShaderProgramResourceLoadedEvent(const Event::ResourceLoadedEvent<ShaderProgram>& event);

		/**
		 * Handles ResourceLoadedEvents for textures by
		 * executing the callback function.
		 *
		 * @note This method is executed from the main game thread.
		 *
		 * @param event Reference to the event.
		 */
		void HandleTextureResourceLoadedEvent(const Event::ResourceLoadedEvent<Texture>& event);

		/**
		 * Handles ResourceLoadedEvents for audio sources by
		 * executing the callback function.
		 *
		 * @note This method is executed from the main game thread.
		 *
		 * @param event Reference to the event.
		 */
		void HandleAudioResourceLoadedEvent(const Event::ResourceLoadedEvent<IAudioSource>& event);

	private:
		/**
		 * Returns the unique name that should identify the shader program
		 * specified by the provided vertex and fragment shader file paths.
		 *
		 * @param vertexShaderFilepath Path to the vertex shader.
		 * @param fragmentShaderFilepath Path to the fragment shader.
		 * @return Unique name for the shader program.
		 */
		static std::string GetShaderProgramResourceName(std::string vertexShaderFilepath,
			std::string fragmentShaderFilepath);

		/**
		 * Returns the unique name that should identify the model specified by
		 * the provided model file path.
		 *
		 * @param filepath Path to the model.
		 * @return Unique name for the model.
		 */
		static std::string GetModelResourceName(std::string filepath);

		/**
		 * Returns the unique name that should identify the texture specified by
		 * the provided image file path.
		 *
		 * @param filepath Path to the image.
		 * @return Unique name for the texture.
		 */
		static std::string GetTextureResourceName(std::string filepath);

		/**
		 * Returns the unique name that should identify the audio source
		 * specified by the provided audio file path.
		 *
		 * @param filepath Path to the audio file.
		 * @return Unique name for the audio source.
		 */
		static std::string GetAudioResourceName(std::string filepath);

	private:
		/**
		 * Event receiver for this thread.
		 */
		std::shared_ptr<ThreadEventReceiver> m_gameThreadEventReceiver;

		/**
		 * Loaded shader programs.
		 */
		std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_shaderPrograms;

		/**
		 * Loaded models.
		 */
		std::unordered_map<std::string, std::shared_ptr<Model>> m_models;

		/**
		 * Loaded textures.
		 */
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

		/**
		 * Loaded audio sources.
		 */
		std::unordered_map<std::string, std::shared_ptr<IAudioSource>> m_audioSources;

		/**
		 * The resource loader.
		 */
		ResourceLoader m_resourceLoader;

		/**
		 * The loading thread.
		 */
		std::thread m_loadingThread;

		/**
		 * Subscription identifier for the ResourceLoadedEvent<Model>
		 * subscription.
		 */
		EventDispatcher::SubscriptionID m_modelResourceLoadedSubscription;

		/**
		 * Subscription identifier for the ResourceLoadedEvent<ShaderProgram>
		 * subscription.
		 */
		EventDispatcher::SubscriptionID m_shaderProgramResourceLoadedSubscription;

		/**
		 * Subscription identifier for the ResourceLoadedEvent<Texture>
		 * subscription.
		 */
		EventDispatcher::SubscriptionID m_textureResourceLoadedSubscription;

		/**
		 * Subscription identifier for the ResourceLoadedEvent<IAudioSource>
		 * subscription.
		 */
		EventDispatcher::SubscriptionID m_audioResourceLoadedSubscription;
	};
}

#endif
