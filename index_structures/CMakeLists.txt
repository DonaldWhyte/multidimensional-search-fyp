# Static library containing all implemented index structures
add_library( index_structures
	# Source files
	"src/SequentialScan.cpp"
	"src/Octree.cpp"
	"src/PyramidTree.cpp"
	"src/IndexPseudoPyramidTree.cpp"
	"src/SplayPseudoPyramidTree.cpp"
	"src/PseudoPyramidTree.cpp"
	"src/BoundaryDistanceHashing.cpp"
	"src/KDTree.cpp"
	"src/UniqueHashTable.cpp"
	"src/DuplicateHashTable.cpp"
	"src/BucketKDTree.cpp"
	# Header files
	"include/IndexStructure.h"
	"include/SequentialScan.h"
	"include/Octree.h"
	"include/PyramidTree.h"
	"include/IndexPseudoPyramidTree.h"
	"include/SplayTree.h"
	"include/SplayPseudoPyramidTree.h"
	"include/PseudoPyramidTree.h"
	"include/BoundaryDistanceHashing.h"
	"include/KDTree.h"
	"include/UniqueHashTable.h"
	"include/DuplicateHashTable.h"
	"include/BucketKDTree.h"
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
	"tests/TestIndexPseudoPyramidTree.h"
	"tests/TestSplayTree.h"
	"tests/TestSplayPseudoPyramidTree.h"
	"tests/TestPseudoPyramidTree.h"
	"tests/TestBoundaryDistanceHashing.h"
	"tests/TestKDTree.h"
	"tests/TestUniqueHashTable.h"
	"tests/TestDuplicateHashTable.h"
	"tests/TestBucketKDTree.h"
)
# Ensure google-test library is linked and include files are in search path
# NOTE: index_structures library is dependant on the 'data' library for the
# necessary types
target_link_libraries ( structure_tests index_structures data ${LIBS} )
include_directories ({DEPENDENCIES})