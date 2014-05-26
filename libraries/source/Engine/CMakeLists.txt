# Library name.
set(LIBRARY_NAME Engine)

# Set project include & source root directory path variables.
set(INC_ROOT ${PROJECT_SOURCE_DIR}/include/Engine)
set(SRC_ROOT ${PROJECT_SOURCE_DIR}/source/Engine)

# Include headers.
include_directories(${PROJECT_SOURCE_DIR}/include)

# Files.
set(ENGINE_SRCS
	${INC_ROOT}/NonCopyable.hpp

	${INC_ROOT}/Rectangle.hpp
	${INC_ROOT}/Cuboid.hpp
	${INC_ROOT}/BezierCurve.hpp
	${INC_ROOT}/Octree.hpp

	${INC_ROOT}/ThreadEventReceiver.hpp
	${SRC_ROOT}/ThreadEventReceiver.cpp

	${INC_ROOT}/Application.hpp
	${SRC_ROOT}/Application.cpp

	${INC_ROOT}/Audio.hpp
	${SRC_ROOT}/Audio.cpp

	${INC_ROOT}/IAudioSource.hpp
	${SRC_ROOT}/IAudioSource.cpp

	${INC_ROOT}/WaveFile.hpp
	${SRC_ROOT}/WaveFile.cpp

	${INC_ROOT}/Window.hpp
	${SRC_ROOT}/Window.cpp

	${INC_ROOT}/Renderer.hpp
	${SRC_ROOT}/Renderer.cpp

	${INC_ROOT}/Collider.hpp
	${SRC_ROOT}/Collider.cpp

	${INC_ROOT}/ResourceManager.hpp
	${SRC_ROOT}/ResourceManager.cpp

	${INC_ROOT}/SceneStack.hpp
	${SRC_ROOT}/SceneStack.cpp

	${INC_ROOT}/IScene.hpp
	${SRC_ROOT}/IScene.cpp

	${INC_ROOT}/IGameScene.hpp
	${SRC_ROOT}/IGameScene.cpp

	${INC_ROOT}/GameObject.hpp
	${SRC_ROOT}/GameObject.cpp

	${INC_ROOT}/EventDispatcher.hpp
	${SRC_ROOT}/EventDispatcher.cpp

	${INC_ROOT}/Event/IEvent.hpp
	${SRC_ROOT}/Event/IEvent.cpp

	${INC_ROOT}/Event/PushSceneEvent.hpp
	${SRC_ROOT}/Event/PushSceneEvent.cpp

	${INC_ROOT}/Event/PopSceneEvent.hpp
	${SRC_ROOT}/Event/PopSceneEvent.cpp

	${INC_ROOT}/Event/KeyPressedEvent.hpp
	${SRC_ROOT}/Event/KeyPressedEvent.cpp

	${INC_ROOT}/Event/MouseButtonPressedEvent.hpp
	${SRC_ROOT}/Event/MouseButtonPressedEvent.cpp

	${INC_ROOT}/Event/CreateGameObjectEvent.hpp
	${SRC_ROOT}/Event/CreateGameObjectEvent.cpp

	${INC_ROOT}/Event/ChildGameObjectAttachedEvent.hpp
	${SRC_ROOT}/Event/ChildGameObjectAttachedEvent.cpp

	${INC_ROOT}/Event/AncestorTransformTranslatedEvent.hpp
	${SRC_ROOT}/Event/AncestorTransformTranslatedEvent.cpp

	${INC_ROOT}/Event/AncestorTransformRotatedEvent.hpp
	${SRC_ROOT}/Event/AncestorTransformRotatedEvent.cpp

	${INC_ROOT}/Event/AncestorTransformScaledEvent.hpp
	${SRC_ROOT}/Event/AncestorTransformScaledEvent.cpp

	${INC_ROOT}/Event/CollisionEvent.hpp
	${SRC_ROOT}/Event/CollisionEvent.cpp

	${INC_ROOT}/Event/ResourceLoadedEvent.hpp

	${INC_ROOT}/Event/LoadModelResourceEvent.hpp
	${SRC_ROOT}/Event/LoadModelResourceEvent.cpp

	${INC_ROOT}/Event/LoadShaderProgramResourceEvent.hpp
	${SRC_ROOT}/Event/LoadShaderProgramResourceEvent.cpp

	${INC_ROOT}/Event/LoadTextureResourceEvent.hpp
	${SRC_ROOT}/Event/LoadTextureResourceEvent.cpp

	${INC_ROOT}/Event/LoadAudioResourceEvent.hpp
	${SRC_ROOT}/Event/LoadAudioResourceEvent.cpp

	${INC_ROOT}/Event/WindowResizeEvent.hpp
	${SRC_ROOT}/Event/WindowResizeEvent.cpp

	${INC_ROOT}/Event/DestroyGameObjectEvent.hpp
	${SRC_ROOT}/Event/DestroyGameObjectEvent.cpp

	${INC_ROOT}/UI/IShape.hpp
	${SRC_ROOT}/UI/IShape.cpp

	${INC_ROOT}/UI/Rectangle.hpp
	${SRC_ROOT}/UI/Rectangle.cpp

	${INC_ROOT}/UI/Glyph.hpp
	${SRC_ROOT}/UI/Glyph.cpp

	${INC_ROOT}/UI/Font.hpp
	${SRC_ROOT}/UI/Font.cpp

	${INC_ROOT}/UI/Text.hpp
	${SRC_ROOT}/UI/Text.cpp

	${INC_ROOT}/UI/Renderer.hpp
	${SRC_ROOT}/UI/Renderer.cpp

	${INC_ROOT}/stb_image/stb_image.h
	${SRC_ROOT}/stb_image/stb_image.c

	${INC_ROOT}/Texture.hpp
	${SRC_ROOT}/Texture.cpp

	${INC_ROOT}/Shader.hpp
	${SRC_ROOT}/Shader.cpp

	${INC_ROOT}/ShaderProgram.hpp
	${SRC_ROOT}/ShaderProgram.cpp

	${INC_ROOT}/Model.hpp
	${SRC_ROOT}/Model.cpp

	${INC_ROOT}/Ray.hpp
	${SRC_ROOT}/Ray.cpp

	${INC_ROOT}/Plane.hpp
	${SRC_ROOT}/Plane.cpp

	${INC_ROOT}/IGameObjectFactory.hpp

	${INC_ROOT}/Attribute/IAttribute.hpp
	${SRC_ROOT}/Attribute/IAttribute.cpp

	${INC_ROOT}/Attribute/Transform.hpp
	${SRC_ROOT}/Attribute/Transform.cpp

	${INC_ROOT}/Attribute/OrthographicCamera.hpp
	${SRC_ROOT}/Attribute/OrthographicCamera.cpp

	${INC_ROOT}/Attribute/ShaderProgram.hpp
	${SRC_ROOT}/Attribute/ShaderProgram.cpp

	${INC_ROOT}/Attribute/Model.hpp
	${SRC_ROOT}/Attribute/Model.cpp

	${INC_ROOT}/Attribute/BoundingSphere.hpp
	${SRC_ROOT}/Attribute/BoundingSphere.cpp

	${INC_ROOT}/Attribute/DirectionalLight.hpp
	${SRC_ROOT}/Attribute/DirectionalLight.cpp

	${INC_ROOT}/Behaviour/IBehaviour.hpp
	${SRC_ROOT}/Behaviour/IBehaviour.cpp
)

# Create a static library.
add_library(${LIBRARY_NAME} STATIC ${ENGINE_SRCS})

# GLEW
find_package(GLEW REQUIRED glew)
include_directories(${GLEW_INCLUDE_PATH})
target_link_libraries(${LIBRARY_NAME} ${GLEW_LIBRARIES})

# Find OpenGL and GLU.
find_package(OpenGL REQUIRED)
if(NOT APPLE)
	include_directories(${OPENGL_INCLUDE_DIR})
endif()
target_link_libraries(${LIBRARY_NAME} ${OPENGL_LIBRARIES})

# GLFW
find_package(GLFW3 REQUIRED glfw3)
include_directories(${GLFW3_INCLUDE_PATH})
target_link_libraries(${LIBRARY_NAME} ${GLFW3_LIBRARIES})

# ASSIMP
find_package(ASSIMP REQUIRED assimp)
include_directories(${ASSIMP_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} ${ASSIMP_LIBRARIES})

# FreeType
find_package(FREETYPE2 REQUIRED freetype2)
include_directories(${FREETYPE2_INCLUDE_DIRS})
target_link_libraries(${LIBRARY_NAME} ${FREETYPE2_LIBRARIES})

# PortAudio
find_package(PORTAUDIO REQUIRED portaudio)
include_directories(${PORTAUDIO_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} ${PORTAUDIO_LIBRARIES})

# Install.
install(TARGETS ${LIBRARY_NAME} DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include
		DESTINATION .
		PATTERN ".svn" EXCLUDE
		PATTERN ".DS_Store" EXCLUDE)
