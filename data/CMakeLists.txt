# Static library containing all data generators/loaders
add_library( data
	# Source files
	"src/Dummy.cpp"
	# Header files
	"include/DataTypes.h"
	"include/Point.h"
	"include/Region.h"
)
# Ensure include directory is in search path
include_directories ("include")

# Executable for running unit and/or integration tests
add_executable ( data_tests
	"tests/TestRunner.cpp"
)
# Ensure google-test library is linked and include files are in search path
target_link_libraries ( data_tests ${LIBS} )
include_directories ({DEPENDENCIES})