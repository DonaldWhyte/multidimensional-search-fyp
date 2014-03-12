# Executable for main evaluation program
set (EVALUATOR_SRC 
	# Source files
	"src/CommandLineArguments.cpp"
	"src/IndexStructureFactory.cpp"
	"src/TestOperation.cpp"
	"src/TestOperationLoader.cpp"
	# Header files
	"include/IndexStructureFactory.h"
	"include/CommandLineArguments.h"
	"include/TestOperation.h"
	"include/TestOperationLoader.h"
)
add_executable( evaluator
	"src/EvaluatorMain.cpp"
	${EVALUATOR_SRC}
)
# Ensure include directory is in search path
include_directories ("include")
# Link both the data and index structure libraries to evaluator
target_link_libraries (evaluator data)
include_directories(${PROJECT_ROOT}/data/include)
target_link_libraries (evaluator index_structures)
include_directories(${PROJECT_ROOT}/index_structures/include)

# Executable for running unit and/or integration tests
add_executable ( evaluator_tests
	"tests/TestRunner.cpp"
	"tests/TestCommandLineArguments.h"
	"tests/TestTestOperationLoader.h"
	${EVALUATOR_SRC}
)
# Ensure google-test library is linked and include files are in search path
# NOTE: index_structures library is dependant on the 'data' library for the
# necessary types
target_link_libraries ( evaluator_tests index_structures data ${LIBS} )
include_directories ({DEPENDENCIES})

# Add Boost as dependency for evaluator and the unit test
target_link_libraries(evaluator ${Boost_LIBRARIES})
target_link_libraries(evaluator_tests ${Boost_LIBRARIES})

# Copy additional test files to build directory
file( COPY tests/test_files DESTINATION ${BUILD_DIR} )