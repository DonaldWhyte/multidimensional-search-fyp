# Executable for main evaluation program
set (EVALUATOR_SRC 
	# Source files
	"src/CommandLineArguments.cpp"
	"src/TestSuite.cpp"
	"src/Timing.cpp"
	"src/IndexStructureFactory.cpp"
	"src/TestOperation.cpp"
	"src/TestOperationLoader.cpp"
	"src/SpecEvaluator.cpp"
	# Header files
	"include/IndexStructureFactory.h"
	"include/CommandLineArguments.h"
	"include/TestSuite.h"
	"include/Timing.h"
	"include/TestOperation.h"
	"include/TestOperationLoader.h"
	"include/SpecEvaluator.h"
)
add_executable( evaluator
	"src/EvaluatorMain.cpp"
	${EVALUATOR_SRC}
)
# Ensure include directory is in search path
include_directories ("include")
# Link both the data and index structure libraries to evaluator
target_link_libraries (evaluator data index_structures ${LIBS})
include_directories(${PROJECT_ROOT}/data/include)
include_directories(${PROJECT_ROOT}/index_structures/include)

# Executable for running unit and/or integration tests
add_executable ( evaluator_tests
	"tests/TestRunner.cpp"
	"tests/TestCommandLineArguments.h"
	"tests/TestTestSuite.h"
	"tests/TestTiming.h"
	"tests/TestTestOperationLoader.h"
	${EVALUATOR_SRC}
)
# Ensure google-test library is linked and include files are in search path
# NOTE: index_structures library is dependent on the 'data' library for the
# necessary types
target_link_libraries ( evaluator_tests index_structures data ${LIBS} )
include_directories ({DEPENDENCIES})

# Add Boost as dependency for evaluator and the unit tests
target_link_libraries(evaluator ${Boost_LIBRARIES})
target_link_libraries(evaluator_tests ${Boost_LIBRARIES})

# Copy additional test files to build directory
file( COPY tests/test_files DESTINATION ${BUILD_DIR} )
