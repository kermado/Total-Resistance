# Project Name.
project(Engine)

# Subdirectories.
add_subdirectory(source)

# Unit tests.
if (tests)
	add_subdirectory(tests)
endif()
