#
# Try to find PortAudio library and include path.
# Once done this will define
#
# PORTAUDIO_FOUND
# PORTAUDIO_INCLUDE_PATH
# PORTAUDIO_LIBRARY
#

FIND_PATH(PORTAUDIO_INCLUDE_PATH portaudio.h
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	${PORTAUDIO_ROOT_DIR}/include
	DOC "The directory where portaudio.h resides")

# Prefer the static library.
FIND_LIBRARY(PORTAUDIO_LIBRARY
	NAMES
		libportaudio.a
		libportaudio.so
		libportaudio.dylib
		libportaudio.2.dylib
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		${PORTAUDIO_ROOT_DIR}/lib
	DOC
		"The GLEW library")

SET(PORTAUDIO_FOUND "NO")
IF(PORTAUDIO_INCLUDE_PATH AND PORTAUDIO_LIBRARY)
	SET(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARY})
	SET(PORTAUDIO_FOUND "YES")
	message(STATUS "Found PortAudio")

	IF(APPLE)
		MESSAGE(STATUS "Linking PortAudio with CoreAudio, AudioToolbox and AudioUnit...")
		find_library(COREAUDIO NAMES CoreAudio)
		find_library(AUDIOTOOLBOX NAMES AudioToolbox)
		find_library(AUDIOUNIT NAMES AudioUnit)
		SET(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARIES} ${COREAUDIO} ${AUDIOTOOLBOX} ${AUDIOUNIT})
	ELSEIF(UNIX)
		MESSAGE(STATUS "Linking PortAudio with ALSA...")
		find_library(ALSA NAMES asound)
		SET(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARIES} ${ALSA})
	ENDIF()
ENDIF()
