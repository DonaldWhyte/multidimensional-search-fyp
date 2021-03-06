# Static library containing all data generators/loaders
add_library( data
	# Source files
	"src/Util.cpp"
	"src/Point.cpp"
	"src/Region.cpp"
	"src/PointFactory.cpp"
	"src/DatasetFileLoader.cpp"
	"src/UniformDatasetGenerator.cpp"
	"src/RandomPointGenerator.cpp"
	"src/ClusteredDatasetGenerator.cpp"
	"src/SkewedDatasetGenerator.cpp"
	# Header files
	"include/DataTypes.h"
	"include/Util.h"
	"include/Point.h"
	"include/Region.h"
	"include/PointFactory.h"
	"include/DatasetFileLoader.h"
	"include/UniformDatasetGenerator.h"
	"include/RandomPointGenerator.h"
	"include/ClusteredDatasetGenerator.h"
	"include/SkewedDatasetGenerator.h"
)
# Ensure include directory is in search path
include_directories ("include")

# Executable for running unit and/or integration tests
add_executable ( data_tests
	"tests/TestRunner.cpp"
	"tests/TestTypes.h"
	"tests/TestPointFactory.h"
	"tests/TestDatasetFileLoader.h"
	"tests/TestDatasetGenerators.h"
)
include_directories ("tests")
# Ensure google-test library is linked and include files are in search path
target_link_libraries ( data_tests data ${LIBS} )
include_directories (${DEPENDENCIES})

# Executable for running core data gennerator, which is used to generated
# a varied set of test datasets for evaluating structures
add_executable(data_generator "src/EvaluationDataGenerator.cpp")
target_link_libraries( data_generator data)

# Copy additional test files to build directory
file( COPY tests/test_files DESTINATION ${BUILD_DIR} )