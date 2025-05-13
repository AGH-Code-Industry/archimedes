# TESTS SOURCE FILES
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

# TESTS EXECUTABLE TARGET
add_executable(ArchimedesTests ${TEST_SOURCES})

# LINKING DEPENDENCIES FOR TESTS
target_link_libraries(ArchimedesTests PRIVATE Archimedes gtest_main)

# TARGET CXX STANDARD
target_compile_features(ArchimedesTests PRIVATE cxx_std_23)

# REGISTER TESTS FOR CTEST
add_test(NAME ArchimedesTests COMMAND ArchimedesTests)
