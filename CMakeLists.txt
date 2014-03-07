cmake_minimum_required (VERSION 2.8)

# Define project name and version
project (MultidimensionalSearch)
set (MDSEARCH_MAJOR_VERSION 0)
set (MDSEARCH_MINOR_VERSION 0)

# Give names for directories in the project
set (PROJECT_ROOT ${PROJECT_SOURCE_DIR})
set (DEPENDENCIES ${PROJECT_ROOT}/dependencies)
set (BUILD_DIR ${PROJECT_ROOT}/build)

# Add library search directory and include directories 
include_directories(${DEPENDENCIES})
# Add static libraries of all dependencies here
set( LIBS
	${DEPENDENCIES}/libgtest.a
	pthread # for google-test
)

# Set directory all build files go to
set_property (GLOBAL PROPERTY RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
set (EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})
set (LIBRARY_OUTPUT_PATH ${BUILD_DIR})

# Set compiler flags
set( EXTRA_CXX_COMPILER_FLAGS -fno-exceptions ) # exceptions are being disabled!
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_COMPILER_FLAGS}" )

# Process all sub-projects
add_subdirectory(index_structures)
add_subdirectory(data)
add_subdirectory(evaluator)