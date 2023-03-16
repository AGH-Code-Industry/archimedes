cmake_minimum_required(VERSION 3.19)

project(
  archimedes
  VERSION 0.1.0
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
if (MSVC)
    add_compile_options("/Zc:__cplusplus")
endif (MSVC)

set(DEPENDENCIES extern)

add_library(glad)
target_sources(glad PRIVATE ${DEPENDENCIES}/glad/src/glad.c)
target_include_directories(glad PRIVATE ${DEPENDENCIES}/glad/include)
add_library(${PROJECT_NAME})
file(GLOB_RECURSE ARCHIMEDES_SOURCE src/*.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${ARCHIMEDES_SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC include ${DEPENDENCIES}/glm ${DEPENDENCIES}/glad/include ${DEPENDENCIES}/glfw/include)
link_directories(${DEPENDENCIES}/GLFW/lib-vc2022)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw3 glad)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850 #1.12.x branch last commit
)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#^ according to docs it might be helpful on Windows. Gonna leave it commented
#for now
#TODO: Compiling unit tests should be optional. I have not implemented it for now
#      because why bother?
FetchContent_MakeAvailable(googletest)
enable_testing()
set(TESTS_NAME test_${PROJECT_NAME})
file(GLOB_RECURSE TEST_ARCHIMEDES_SOURCE tests/**.cpp)
add_executable(${TESTS_NAME})
target_sources(${TESTS_NAME} PRIVATE ${TEST_ARCHIMEDES_SOURCE})
target_include_directories(${TESTS_NAME} PRIVATE include)
target_link_libraries(${TESTS_NAME} PRIVATE ${PROJECT_NAME} GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${TESTS_NAME})
