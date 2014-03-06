# Executable for main evaluation program
file( GLOB LIB_SOURCES src/*.cpp )
file( GLOB LIB_HEADERS include/*.h )
add_executable( evaluator ${LIB_SOURCES} ${LIB_HEADERS} )

# Link both the data and index structure libraries to evaluator
target_link_libraries (evaluator data)
include_directories({PROJECT_ROOT}/data/include)
target_link_libraries (evaluator index_structures)
include_directories({PROJECT_ROOT}/index_structures/include)