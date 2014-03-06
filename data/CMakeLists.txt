# Static library containing all data generators/loaders
file( GLOB LIB_SOURCES src/*.cpp )
file( GLOB LIB_HEADERS include/*.h )
add_library( data ${LIB_SOURCES} ${LIB_HEADERS} )

# Executable for running unit and/or integration tests
file( GLOB TEST_SOURCES tests/*.h tests/*.cpp )
add_executable ( data_tests ${TEST_SOURCES} )
# Ensure google-test library is linked and include files are in search path
target_link_libraries ( data_tests ${LIBS} )
include_directories ({DEPENDENCIES})