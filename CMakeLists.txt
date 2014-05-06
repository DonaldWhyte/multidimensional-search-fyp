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
	pthread
	rt
)
# If current machine is a lab machine where the main performance
# tests of the project are being run on, then use the libraries
# specificially build for the lab.
# NOTE: Linking tcmalloc to ALL executables and libraries so they
# all benefit from a faster malloc().
if(DEFINED ENV{LAB_MACHINE})
	set ( LIBS ${LIBS} ${DEPENDENCIES}/lab_libs/libprofiler.a ${DEPENDENCIES}/lab_libs/libtcmalloc.a )
	set ( LIBS ${LIBS} ${DEPENDENCIES}/lab_libs/libboost_program_options.a )
else()
	set ( LIBS ${LIBS} ${DEPENDENCIES}/libprofiler.a ${DEPENDENCIES}/libtcmalloc.a )

	# Search for Boost library
	set(Boost_USE_STATIC_LIBS ON) 
	set(Boost_USE_MULTITHREADED ON)  
	set(Boost_USE_STATIC_RUNTIME OFF) 
	find_package(Boost 1.41.0 COMPONENTS program_options REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib/)
endif()

# Set directory all build files go to
set_property (GLOBAL PROPERTY RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
set (EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})
set (LIBRARY_OUTPUT_PATH ${BUILD_DIR})

# Set compiler flags
set( EXTRA_CXX_COMPILER_FLAGS "-O3" )
# This is added rather than manually specifying the phread library with
# "-lpthread" since it requires some special compilation steps (in GCC)
set( EXTRA_CXX_COMPILER_FLAGS "${EXTRA_CXX_COMPILER_FLAGS} -pthread" )
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_COMPILER_FLAGS}" )

# Process all sub-projects
add_subdirectory(index_structures)
add_subdirectory(data)
add_subdirectory(evaluator)
add_subdirectory(mdsearch)