cmake_minimum_required(VERSION 3.19)

project(
  archimedes
  VERSION 0.1.0
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)

set(DEPENDENCIES extern)

add_library(glad)
target_sources(glad PRIVATE ${DEPENDENCIES}/glad/src/glad.c)
target_include_directories(glad PRIVATE ${DEPENDENCIES}/glad/include)

add_library(${PROJECT_NAME})
file(GLOB ARCHIMEDES_SOURCE src/*.cpp src/exceptions/*.cpp src/ecs/*.cpp src/ecs/components/*.cpp src/ecs/systems/*.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${ARCHIMEDES_SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC include ${DEPENDENCIES}/glm ${DEPENDENCIES}/glad/include ${DEPENDENCIES}/json/include ${DEPENDENCIES}/spdlog/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_subdirectory(${DEPENDENCIES}/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(extern/GLFW/include)
link_directories(extern/GLFW/lib-mingw-w64)

set(TESTS_NAME test_${PROJECT_NAME})
file(GLOB TEST_ARCHIMEDES_SOURCE tests/**.cpp)
add_executable(${TESTS_NAME})
target_sources(${TESTS_NAME} PRIVATE ${TEST_ARCHIMEDES_SOURCE})
target_include_directories(${TESTS_NAME} PRIVATE include)
target_link_libraries(${TESTS_NAME} PRIVATE ${PROJECT_NAME} GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${TESTS_NAME})
