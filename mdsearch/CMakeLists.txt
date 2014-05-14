# Ensure include directory is in search path
include_directories ("include")

# Executable for testing implemented structures
add_executable(mdsearch_tests
	# Source files
	"src/test_structures.cpp"
)
target_link_libraries ( mdsearch_tests rt )

# Executable for timing implemented structures on real program traces
add_executable(mdsearch_trace_timings
	# Source files
	"src/test_trace.cpp"
)
# Add runtime library to test program
target_link_libraries ( mdsearch_trace_timings rt )