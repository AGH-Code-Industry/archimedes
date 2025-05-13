# SOURCE FILES
file (GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

# EXAMPLES EXECUTABLE TARGET
add_executable(ArchimedesExamples ${EXAMPLE_SOURCES})

# LINKING ENGINE TO EXAMPLES
target_link_libraries(ArchimedesExamples PRIVATE Archimedes)

# CXX STANDARD FOR EXAMPLES TARGET
target_compile_features(ArchimedesExamples PRIVATE cxx_std_23)

# CWD FOR PROJECT
set_target_properties(ArchimedesExamples PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/examples")