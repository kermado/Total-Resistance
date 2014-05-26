cmake_minimum_required(VERSION 2.8)

# Options
option(tests "Build all unit tests" OFF)

# Output directories.
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Project Name.
PROJECT(Prototype1)

# Use c++11 in all cases.
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Clang specific compile flags.
IF(CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	IF(APPLE)
		# Use libc++.
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
		# Library header include paths.
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include")
	ELSE()
		# C++ header include paths.
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/c++/4.8.1")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/c++/4.8.1/x86_64-redhat-linux")
	ENDIF()
ELSE()
	# The gcc/g++ compiler is not supported due to a bug in the compilers implementation
	# of capturing variadic templates in lambda functions:
	# http://gcc.gnu.org/bugzilla/show_bug.cgi?id=41933
	message(FATAL_ERROR "Sorry, clang is the only supported compiler at this time.")
ENDIF()

# CMake modules path.
SET(CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/cmake/modules
	${CMAKE_MODULE_PATH}
)

# Subdirectories.
ADD_SUBDIRECTORY(libraries)
ADD_SUBDIRECTORY(game)

# Install resources.
INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/resources" DESTINATION .)

# Package.
INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple 3D tower defense game.")
SET(CPACK_PACKAGE_VENDOR "Omar Kermad")
SET(CPACK_PACKAGE_EXECUTABLES "Game" "The Game")
SET(CPACK_BUNDLE_NAME "TowerDefense")
SET(CPACK_BUNDLE_PLIST "${PROJECT_SOURCE_DIR}/Info.plist")
SET(CPACK_BUNDLE_ICON "${PROJECT_SOURCE_DIR}/Icon.icns")
SET(CPACK_GENERATOR "Bundle")
INCLUDE(CPack)