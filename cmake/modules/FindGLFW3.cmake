#
# Try to find GLFW library and include path.
# Once done this will define
#
# GLFW3_FOUND
# GLFW3_INCLUDE_PATH
# GLFW3_LIBRARY
#

FIND_PATH(GLFW3_INCLUDE_PATH GLFW/glfw3.h
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	${GLFW3_ROOT_DIR}/include
	DOC "The directory where GLFW/glfw3.h resides")

# Prefer the static library.
FIND_LIBRARY(GLFW3_LIBRARY
	NAMES
		libGLFW3.a
		libglfw3.a
		libGLFW.a
		libglfw.a
		libGLFW.so
		libglfw.so
		libGLFW.so.3
		libglfw.so.3
		libGLFW.so.3.0
		libglfw.so.3.0
		libGLFW3.dylib
		libglfw3.dylib
		libglfw.dylib
		libGLFW.3.dylib
		libglfw.3.dylib
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		${GLFW_ROOT_DIR}/lib
	DOC
		"The GLFW library")

SET(GLFW3_FOUND "NO")
IF(GLFW3_INCLUDE_PATH AND GLFW3_LIBRARY)
	SET(GLFW3_LIBRARIES ${GLFW3_LIBRARY})
	SET(GLFW3_FOUND "YES")
    MESSAGE(STATUS "Found GLFW3")

	# OS X uses the Cocoa port so we need to link against Cocoa.
	# This is only required when linking with the static GLFW3 library.
	IF(APPLE)
		MESSAGE(STATUS "Linking GLFW3 with Cocoa, IOKit and CoreVideo...")
		find_library(COREVIDEO NAMES CoreVideo)
		find_library(IOKIT NAMES IOKit)
		find_library(COCOA NAMES Cocoa)
		SET(GLFW3_LIBRARIES ${GLFW3_LIBRARIES} ${COREVIDEO} ${IOKIT} ${COCOA})
	ELSEIF(UNIX)
		MESSAGE(STATUS "Linking GLFW3 with X11, Xi, Xxf86vm and Xrandr...")
		FIND_LIBRARY(X11 NAMES X11)
		FIND_LIBRARY(Xi NAMES Xi)
		FIND_LIBRARY(Xxf86vm NAMES Xxf86vm)
		FIND_LIBRARY(Xrandr NAMES Xrandr)
		SET(GLFW3_LIBRARIES ${GLFW3_LIBRARIES} ${X11} ${Xi} ${Xxf86vm} ${Xrandr})
	ENDIF()
ENDIF()
