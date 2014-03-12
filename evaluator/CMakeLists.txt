# Executable for main evaluation program
add_executable( evaluator
	# Source files
	"src/EvaluatorMain.cpp"
	"src/IndexStructureFactory.cpp"
	# Header files
	"include/IndexStructureFactory.h"
)
# Ensure include directory is in search path
include_directories ("include")

# Link both the data and index structure libraries to evaluator
target_link_libraries (evaluator data)
include_directories(${PROJECT_ROOT}/data/include)
target_link_libraries (evaluator index_structures)
include_directories(${PROJECT_ROOT}/index_structures/include)