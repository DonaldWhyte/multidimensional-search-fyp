cmake_minimum_required (VERSION 2.6)

# Define project name and version
project (MultidimensionalSearch)
set (MDSEARCH_MAJOR_VERSION 0)
set (MDSEARCH_MINOR_VERSION 0)

# Give names for directories in the project
set (PROJECT_ROOT ${PROJECT_SOURCE_DIR})
set (DEPENDENCIES ${PROJECT_ROOT}/dependencies)

# Add library search directory and include directories 
include_directories(${DEPENDENCIES})
# Add static libraries of all dependencies here
set( LIBS
	${DEPENDENCIES}/libgtest.a
	pthread # for google-test
)

set_property (GLOBAL PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT}/build)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT}/build)
set (LIBRARY_OUTPUT_PATH ${PROJECT_ROOT}/build) 

# Process all sub-projects
add_subdirectory(index_structures)
add_subdirectory(data)
add_subdirectory(evaluator)