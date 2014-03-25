# Static library containing all implemented index structures
add_library( index_structures
	# Source files
	"src/SequentialScan.cpp"
	"src/Octree.cpp"
	"src/PyramidTree.cpp"
	"src/SplayQuadtree.cpp"
	# Header files
	"include/IndexStructure.h"
	"include/SequentialScan.h"
	"include/Octree.h"
	"include/PyramidTree.h"
	"include/SplayQuadtree.h"
	"include/SplayTree.h"
)
# Ensure include directory is in search path
include_directories ("include")
include_directories ("${PROJECT_ROOT}/data/include") # added path to data library's headers

# Executable for running unit and/or integration tests
add_executable ( structure_tests
	"tests/Common.h"
	"tests/Common.cpp"
	"tests/TestRunner.cpp"
	"tests/TestSequentialScan.h"
	"tests/TestOctree.h"
	"tests/TestPyramidTree.h"
	"tests/TestSplayQuadtree.h"
	"tests/TestSplayTree.h"
)
# Ensure google-test library is linked and include files are in search path
# NOTE: index_structures library is dependant on the 'data' library for the
# necessary types
target_link_libraries ( structure_tests index_structures data ${LIBS} )
include_directories ({DEPENDENCIES})