# Static library containing all implemented index structures
file( GLOB LIB_SOURCES src/*.cpp )
file( GLOB LIB_HEADERS include/*.h )
add_library( index_structures ${LIB_SOURCES} ${LIB_HEADERS} )

# Executable for running unit and/or integration tests
file( GLOB TEST_SOURCES tests/*.h tests/*.cpp )
add_executable ( structure_tests ${TEST_SOURCES} )
# Ensure google-test library is linked and include files are in search path
target_link_libraries ( structure_tests ${LIBS} )
include_directories ({DEPENDENCIES})