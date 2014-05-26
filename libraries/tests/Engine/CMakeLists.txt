# Name for the unit tests target.
set(PROJECT_TESTS_NAME ${PROJECT_NAME}Tests)

# Path to the unit test sources root directory.
set(SRC_ROOT ${PROJECT_SOURCE_DIR}/tests/Engine)

# Include Engine headers.
include_directories(${PROJECT_SOURCE_DIR}/include)

# Unit test sources.
set(TEST_SRCS
	${SRC_ROOT}/EventDispatcherTest.cpp
)

# Add the unit tests executable.
add_executable(${PROJECT_TESTS_NAME} ${TEST_SRCS})

# Link the unit tests executable with the Engine library.
target_link_libraries(${PROJECT_TESTS_NAME} Engine)
