# Static library containing all data generators/loaders
add_library( data
	# Source files
	"src/Point.cpp"
	"src/Region.cpp"
	"src/DatasetFileLoader.cpp"
	"src/UniformDatasetGenerator.cpp"
	"src/HyperCubeRandomGenerator.cpp"
	# Header files
	"include/DataTypes.h"
	"include/Point.h"
	"include/Region.h"
	"include/DatasetFileLoader.h"
	"include/UniformDatasetGenerator.h"
	"include/HyperCubeRandomGenerator.h"
)
# Ensure include directory is in search path
include_directories ("include")

# Executable for running unit and/or integration tests
add_executable ( data_tests
	"tests/TestRunner.cpp"
	"tests/TestTypes.h"
	"tests/TestDatasetFileLoader.h"
	"tests/TestDatasetGenerators.h"
)
include_directories ("tests")
# Ensure google-test library is linked and include files are in search path
target_link_libraries ( data_tests data ${LIBS} )
include_directories ({DEPENDENCIES})

file( COPY tests/test_files DESTINATION ${BUILD_DIR} )