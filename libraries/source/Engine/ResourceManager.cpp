#include <Engine/ResourceManager.hpp>

#include <iostream>

#include <Engine/WaveFile.hpp>

namespace Engine
{
	ResourceLoader::ResourceLoader(std::shared_ptr<ThreadEventReceiver> gameThreadReceiver,
		std::shared_ptr<Window> loadingWindow)
	: m_terminateLoadingThread(false)
	, m_loadingThreadEventReceiver()
	, m_gameThreadEventReceiver(gameThreadReceiver)
	, m_window(loadingWindow)
	, m_loadModelSubscription(0)
	, m_loadShaderProgramSubscription(0)
	, m_loadTextureSubscription(0)
	, m_loadAudioSubscription(0)
	, m_stopThreadSubscription(0)
	{
		// Subscribe to receive loading request events.
		m_loadModelSubscription = m_loadingThreadEventReceiver.Subscribe<Event::LoadModelResourceEvent>(
			CALLBACK(ResourceLoader::HandleLoadModelResourceEvent)
		);
		m_loadShaderProgramSubscription = m_loadingThreadEventReceiver.Subscribe<Event::LoadShaderProgramResourceEvent>(
			CALLBACK(ResourceLoader::HandleLoadShaderProgramResourceEvent)
		);
		m_loadTextureSubscription = m_loadingThreadEventReceiver.Subscribe<Event::LoadTextureResourceEvent>(
			CALLBACK(ResourceLoader::HandleLoadTextureResourceEvent)
		);
		m_loadAudioSubscription = m_loadingThreadEventReceiver.Subscribe<Event::LoadAudioResourceEvent>(
			CALLBACK(ResourceLoader::HandleLoadAudioResourceEvent)
		);

		// Subscribe to receive thread termination request events.
		// This should cause the thread entry function to return.
		m_stopThreadSubscription = m_loadingThreadEventReceiver.Subscribe<Event::StopThreadEvent>(
			CALLBACK(ResourceLoader::HandleStopThreadEvent)
		);
	}

	ResourceLoader::~ResourceLoader()
	{
		// Unsubscribe for events.
		m_loadingThreadEventReceiver.Unsubscribe<Event::LoadModelResourceEvent>(m_loadModelSubscription);
		m_loadingThreadEventReceiver.Unsubscribe<Event::LoadShaderProgramResourceEvent>(m_loadShaderProgramSubscription);
		m_loadingThreadEventReceiver.Unsubscribe<Event::LoadTextureResourceEvent>(m_loadTextureSubscription);
		m_loadingThreadEventReceiver.Unsubscribe<Event::LoadAudioResourceEvent>(m_loadAudioSubscription);
		m_loadingThreadEventReceiver.Unsubscribe<Event::StopThreadEvent>(m_stopThreadSubscription);
	}

	void ResourceLoader::Run()
	{
		// Make the loading window's OpenGL context current to this thread.
		m_window->MakeContextCurrent();

		// Initialize GLEW.
		glewExperimental = GL_TRUE; // OSX fix.
		if (glewInit() != GLEW_OK)
		{
			exit(1); // Critical failure!
		}

		// Process events until a terminate event has been received.
		while (!m_terminateLoadingThread)
		{
			// Process a single event in the queue.
			m_loadingThreadEventReceiver.ProcessSingleEvent();

			// Sleep for a little while to avoid high CPU load.
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	ThreadEventReceiver& ResourceLoader::GetReceiver()
	{
		return m_loadingThreadEventReceiver;
	}

	void ResourceLoader::HandleStopThreadEvent(const Event::StopThreadEvent& event)
	{
		m_terminateLoadingThread = true;
	}

	void ResourceLoader::HandleLoadModelResourceEvent(const Event::LoadModelResourceEvent& event)
	{
		// Create a new empty model.
		std::shared_ptr<Model> model = std::make_shared<Model>();

		// Try to initialize the model by loading it from a file.
		bool success = model->LoadFromFile(event.GetPath());

		// Publish a ResourceLoadedEvent to the game thread receiver.
		m_gameThreadEventReceiver->Enqueue<Event::ResourceLoadedEvent<Model>>(
			event.GetName(),
			success,
			model,
			event.GetCallback()
		);
	}

	void ResourceLoader::HandleLoadShaderProgramResourceEvent(const Event::LoadShaderProgramResourceEvent& event)
	{
		// Create vertex and fragment shaders.
		Shader vertexShader(Shader::Type::VertexShader);
		Shader fragmentShader(Shader::Type::FragmentShader);

		// Load and compile the vertex and fragment shaders.
		if (vertexShader.LoadFromFile(event.GetVertexShaderPath()) &&
			fragmentShader.LoadFromFile(event.GetFragmentShaderPath()))
		{
			// Create a shader program.
			std::shared_ptr<ShaderProgram> shaderProgram = std::make_shared<ShaderProgram>();

			// Attach the shaders to the shader program.
			shaderProgram->AttachShader(vertexShader);
			shaderProgram->AttachShader(fragmentShader);

			// Link the shader program.
			if (shaderProgram->Link())
			{
				m_gameThreadEventReceiver->Enqueue<Event::ResourceLoadedEvent<ShaderProgram>>(
					event.GetName(),
					true,
					shaderProgram,
					event.GetCallback()
				);

				// We are done.
				return;
			}
			else
			{
				std::cout << "Unable to link shaders: " << event.GetVertexShaderPath()
					<< ", " << event.GetFragmentShaderPath() << std::endl;
			}
		}
		else
		{
			std::cout << "Unable to load shaders: " << event.GetVertexShaderPath()
				<< ", " << event.GetFragmentShaderPath() << std::endl;
		}

		// Something went wrong if we reach this point.
		m_gameThreadEventReceiver->Enqueue<Event::ResourceLoadedEvent<ShaderProgram>>(
			event.GetName(),
			false,
			nullptr,
			event.GetCallback()
		);
	}

	void ResourceLoader::HandleLoadTextureResourceEvent(const Event::LoadTextureResourceEvent& event)
	{
		// Create a new empty texture.
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();

		// Try to initialize the texture by loading it from a file.
		bool success = texture->LoadFromFile(event.GetPath());

		// Publish a ResourceLoadedEvent to the game thread receiver.
		m_gameThreadEventReceiver->Enqueue<Event::ResourceLoadedEvent<Texture>>(
			event.GetName(),
			success,
			texture,
			event.GetCallback()
		);
	}

	void ResourceLoader::HandleLoadAudioResourceEvent(const Event::LoadAudioResourceEvent& event)
	{
		const std::string filepath = event.GetPath();
		const std::string extension = filepath.substr(filepath.find_last_of(".") + 1);

		if (extension == "wav")
		{
			// Create a new empty wave file audio source.
			std::shared_ptr<WaveFile> source = std::make_shared<WaveFile>();

			// Try to initialize the audio source by loading it from a file.
			bool success = source->LoadFromFile(filepath);

			// Publish a ResourceLoadedEvent to the game thread receiver.
			m_gameThreadEventReceiver->Enqueue<Event::ResourceLoadedEvent<IAudioSource>>(
				event.GetName(),
				success,
				source,
				event.GetCallback()
			);
		}
		else
		{
			std::cerr << "Error: Unable to load unknown audio file extension: \"" + extension + "\"" << std::endl;
			assert(0);
		}
	}


	ResourceManager::ResourceManager(std::shared_ptr<Window> loadingWindow)
	: m_gameThreadEventReceiver(std::make_shared<ThreadEventReceiver>())
	, m_shaderPrograms()
	, m_models()
	, m_textures()
	, m_audioSources()
	, m_resourceLoader(m_gameThreadEventReceiver, loadingWindow)
	, m_loadingThread(&ResourceLoader::Run, &m_resourceLoader)
	, m_modelResourceLoadedSubscription(0)
	, m_shaderProgramResourceLoadedSubscription(0)
	, m_textureResourceLoadedSubscription(0)
	, m_audioResourceLoadedSubscription(0)
	{
		// Subscribe to receive events in the main game thread.
		m_modelResourceLoadedSubscription = m_gameThreadEventReceiver->Subscribe<Event::ResourceLoadedEvent<Model>>(CALLBACK(ResourceManager::HandleModelResourceLoadedEvent));
		m_shaderProgramResourceLoadedSubscription = m_gameThreadEventReceiver->Subscribe<Event::ResourceLoadedEvent<ShaderProgram>>(CALLBACK(ResourceManager::HandleShaderProgramResourceLoadedEvent));
		m_textureResourceLoadedSubscription = m_gameThreadEventReceiver->Subscribe<Event::ResourceLoadedEvent<Texture>>(CALLBACK(ResourceManager::HandleTextureResourceLoadedEvent));
		m_audioResourceLoadedSubscription = m_gameThreadEventReceiver->Subscribe<Event::ResourceLoadedEvent<IAudioSource>>(CALLBACK(ResourceManager::HandleAudioResourceLoadedEvent));
	}

	ResourceManager::~ResourceManager()
	{
		// Unsubscribe for events.
		m_gameThreadEventReceiver->Unsubscribe<Event::ResourceLoadedEvent<Model>>(m_modelResourceLoadedSubscription);
		m_gameThreadEventReceiver->Unsubscribe<Event::ResourceLoadedEvent<ShaderProgram>>(m_shaderProgramResourceLoadedSubscription);
		m_gameThreadEventReceiver->Unsubscribe<Event::ResourceLoadedEvent<Texture>>(m_textureResourceLoadedSubscription);
		m_gameThreadEventReceiver->Unsubscribe<Event::ResourceLoadedEvent<IAudioSource>>(m_audioResourceLoadedSubscription);

		// Publish a StopThreadEvent to the loading thread's
		// event receiver.
		m_resourceLoader.GetReceiver().Enqueue<Event::StopThreadEvent>();

		// Wait for the loading thread to finish.
		m_loadingThread.join();
	}

	void ResourceManager::Update()
	{
		m_gameThreadEventReceiver->Update();
	}

	void ResourceManager::LoadShaderProgram(std::string vertexShaderFilepath,
		std::string fragmentShaderFilepath,
		std::function<void(const Event::ResourceLoadedEvent<ShaderProgram>&)> callback)
	{
		// Unique name for the shader program.
		const std::string name = GetShaderProgramResourceName(vertexShaderFilepath, fragmentShaderFilepath);

		// Ensure that the shader program has not already been loaded and
		// is not in the process of being loaded.
		assert(m_shaderPrograms.find(name) == m_shaderPrograms.end());

		// Create an null entry in the shader programs map.
		m_shaderPrograms[name] = nullptr;

		// Pubish a LoadShaderProgramResourceEvent so that the loading thread
		// can start on loading the resource.
		m_resourceLoader.GetReceiver().Enqueue<Event::LoadShaderProgramResourceEvent>(
			name,
			vertexShaderFilepath,
			fragmentShaderFilepath,
			callback
		);
	}

	std::shared_ptr<ShaderProgram> ResourceManager::GetShaderProgram(std::string vertexShaderFilepath,
		std::string fragmentShaderFilepath)
	{
		auto iter = m_shaderPrograms.find(GetShaderProgramResourceName(vertexShaderFilepath, fragmentShaderFilepath));
		if (iter != m_shaderPrograms.end())
		{
			std::shared_ptr<ShaderProgram> resource = iter->second;
			return resource;
		}
		else
		{
			return nullptr;
		}
	}

	void ResourceManager::LoadModel(std::string filepath,
		std::function<void(const Event::ResourceLoadedEvent<Model>&)> callback)
	{
		// Unique name for the model.
		const std::string name = GetModelResourceName(filepath);

		// Ensure that the model has not already been loaded and
		// is not in the process of being loaded.
		assert(m_models.find(name) == m_models.end());

		// Create an null entry in the models map.
		m_models[name] = nullptr;

		// Pubish a LoadModelResourceEvent so that the loading thread
		// can start on loading the resource.
		m_resourceLoader.GetReceiver().Enqueue<Event::LoadModelResourceEvent>(name, filepath, callback);
	}

	std::shared_ptr<Model> ResourceManager::GetModel(std::string filepath)
	{
		auto iter = m_models.find(GetModelResourceName(filepath));
		if (iter != m_models.end())
		{
			std::shared_ptr<Model> resource = iter->second;
			return resource;
		}
		else
		{
			return nullptr;
		}
	}

	void ResourceManager::LoadTexture(std::string filepath,
		std::function<void(const Event::ResourceLoadedEvent<Texture>&)> callback)
	{
		// Unique name for the texture.
		const std::string name = GetTextureResourceName(filepath);

		// Ensure that the texture has not already been loaded and
		// is not in the process of being loaded.
		assert(m_textures.find(name) == m_textures.end());

		// Create a null entry in the textures map.
		m_textures[name] = nullptr;

		// Publish a LoadTextureResourceEvent so that the loading
		// thread can start loading the texture.
		m_resourceLoader.GetReceiver().Enqueue<Event::LoadTextureResourceEvent>(name, filepath, callback);
	}

	std::shared_ptr<Texture> ResourceManager::GetTexture(std::string filepath)
	{
		auto iter = m_textures.find(GetTextureResourceName(filepath));
		if (iter != m_textures.end())
		{
			std::shared_ptr<Texture> resource = iter->second;
			return resource;
		}
		else
		{
			return nullptr;
		}
	}

	void ResourceManager::LoadAudio(std::string filepath,
		std::function<void(const Event::ResourceLoadedEvent<IAudioSource>&)> callback)
	{
		// Unique name for the audio source.
		const std::string name = GetAudioResourceName(filepath);

		// Ensure that the audio source has not already been loaded and
		// is not in the process of being loaded.
		assert(m_audioSources.find(name) == m_audioSources.end());

		// Create a null entry in the audio sources map.
		m_audioSources[name] = nullptr;

		// Publish a LoadAudioResourceEvent so that the loading
		// thread can start loading the audio source.
		m_resourceLoader.GetReceiver().Enqueue<Event::LoadAudioResourceEvent>(name, filepath, callback);
	}

	std::shared_ptr<IAudioSource> ResourceManager::GetAudio(std::string filepath)
	{
		auto iter = m_audioSources.find(GetAudioResourceName(filepath));
		if (iter != m_audioSources.end())
		{
			std::shared_ptr<IAudioSource> resource = iter->second;
			return resource;
		}
		else
		{
			return nullptr;
		}
	}

	void ResourceManager::HandleModelResourceLoadedEvent(const Event::ResourceLoadedEvent<Model>& event)
	{
		if (event.WasSuccessful())
		{
			assert(event.GetResource());

			// Save the pointer to the model in the list of model resources.
			m_models[event.GetName()] = event.GetResource();
		}
		else
		{
			// Remove any entry to the resource in the models map.
			m_models.erase(m_models.find(event.GetName()));
		}

		// Execute the callback.
		event.GetCallback()(event);
	}

	void ResourceManager::HandleShaderProgramResourceLoadedEvent(const Event::ResourceLoadedEvent<ShaderProgram>& event)
	{
		if (event.WasSuccessful())
		{
			assert(event.GetResource());

			// Save the pointer to the shader program in the list of
			// shader program resources.
			m_shaderPrograms[event.GetName()] = event.GetResource();
		}
		else
		{
			// Remove any entry to the resource in the shader program map.
			m_shaderPrograms.erase(m_shaderPrograms.find(event.GetName()));
		}

		// Execute the callback.
		event.GetCallback()(event);
	}

	void ResourceManager::HandleTextureResourceLoadedEvent(const Event::ResourceLoadedEvent<Texture>& event)
	{
		if (event.WasSuccessful())
		{
			assert(event.GetResource());

			// Save the pointer to the texture in the list of texture resources.
			m_textures[event.GetName()] = event.GetResource();
		}
		else
		{
			// Remove any entry to the resource in the textures map.
			m_textures.erase(m_textures.find(event.GetName()));
		}

		// Execute the callback.
		event.GetCallback()(event);
	}

	void ResourceManager::HandleAudioResourceLoadedEvent(const Event::ResourceLoadedEvent<IAudioSource>& event)
	{
		if (event.WasSuccessful())
		{
			assert(event.GetResource());

			// Save the pointer to the audio source in the list of audio resources.
			m_audioSources[event.GetName()] = event.GetResource();
		}
		else
		{
			// Remove any entry to the resource in the audio sources map.
			m_audioSources.erase(m_audioSources.find(event.GetName()));
		}

		// Execute the callback.
		event.GetCallback()(event);
	}

	std::string ResourceManager::GetShaderProgramResourceName(std::string vertexShaderFilepath,
		std::string fragmentShaderFilepath)
	{
		return vertexShaderFilepath + fragmentShaderFilepath;
	}

	std::string ResourceManager::GetModelResourceName(std::string filepath)
	{
		return filepath;
	}

	std::string ResourceManager::GetTextureResourceName(std::string filepath)
	{
		return filepath;
	}

	std::string ResourceManager::GetAudioResourceName(std::string filepath)
	{
		return filepath;
	}
}
