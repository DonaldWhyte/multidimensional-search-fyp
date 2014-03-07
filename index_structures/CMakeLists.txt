# Static library containing all implemented index structures
add_library( index_structures
	# Source files
	"src/Dummy.cpp"
	# Header files
)
# Ensure include directory is in search path
include_directories ("include")

# Executable for running unit and/or integration tests
add_executable ( structure_tests
	"tests/TestRunner.cpp"
)
# Ensure google-test library is linked and include files are in search path
target_link_libraries ( structure_tests index_structures ${LIBS} )
include_directories ({DEPENDENCIES})