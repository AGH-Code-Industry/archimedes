cmake_minimum_required(VERSION 3.19)

project(
  archimedes
  VERSION 0.1.0
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#set(DEPENDENCIES extern)

add_library(${PROJECT_NAME})
file(GLOB_RECURSE ARCHIMEDES_SOURCE src/*.cpp | src/*.c)
target_sources(${PROJECT_NAME} PRIVATE ${ARCHIMEDES_SOURCE})

find_package(glfw3 3.3 REQUIRED)
find_package(GTest REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC include ${glad_INCLUDE_DIR} ${assimp_INCLUDE_DIR}
 ${GTest_INCLUDE_DIR} ${glm_INCLUDE_DIR} ${glfw3_INCLUDE_DIR} ${spdlog_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw assimp::assimp spdlog::spdlog)


#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


set(TESTS_NAME test_${PROJECT_NAME})
file(GLOB_RECURSE TEST_ARCHIMEDES_SOURCE tests/**.cpp)
add_executable(${TESTS_NAME})
target_sources(${TESTS_NAME} PRIVATE ${TEST_ARCHIMEDES_SOURCE})
target_include_directories(${TESTS_NAME} PRIVATE include)
target_link_libraries(${TESTS_NAME} PRIVATE ${PROJECT_NAME} GTest::gtest_main GTest::gtest)

#gtest_discover_tests(${TESTS_NAME})

add_executable(${PROJECT_NAME}_bin archimedes_bin/main.cpp)
target_include_directories(${PROJECT_NAME}_bin PUBLIC include)
target_link_libraries(${PROJECT_NAME}_bin PRIVATE ${PROJECT_NAME} spdlog::spdlog)
