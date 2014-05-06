# Static library containing index structures implemented for public release
add_library( mdsearch
	# Source Files
	"src/test.cpp"
	# Header files
	"include/kdtree.h"
	"include/pyramidtree.h"
)
# Ensure include directory is in search path
include_directories ("include")