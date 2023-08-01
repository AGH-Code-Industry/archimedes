cmake_minimum_required(VERSION 3.19)

project(
  archimedes
  VERSION 0.1.0
  LANGUAGES CXX C
)

#if (CMAKE_GENERATOR MATCHES "Visual Studio")
#endif()

set(CMAKE_CXX_STANDARD 20)
if (MSVC)
    add_compile_options("/Zc:__cplusplus")
    include("conan_files/conan_toolchain.cmake") # idk, but works xd
    list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/conan_files)
    include_directories(${CMAKE_INCLUDE_PATH})
endif (MSVC)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_library(${PROJECT_NAME})
file(GLOB_RECURSE ARCHIMEDES_SOURCE src/*.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${ARCHIMEDES_SOURCE})

#set(glfw3_DIR "./")
#set(opengl_system_DIR "./")
#set(glad_DIR "./")
#set(GTest_DIR "./")
#set(assimp_DIR "./")
#set(glm_DIR "./")
#set(spdlog_DIR "./")
#set(minizip_DIR "./")
#set(BZip2_DIR "./")
#set(ZLIB_DIR "./")
#set(pugixml_DIR "./")
#set(zip_DIR "./")
#set(poly2tri_DIR "./") 
#set(RapidJSON_DIR "./")
#set(draco_DIR "./")
#set(clipper_DIR "./")
#set(stb_DIR "./")
#set(openddlparser_DIR "./")
#set(stb_DIR "./")

#list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
#if(MSVC)
#endif(MSVC)

find_package(stb CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED) 
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 3.3 CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC include ${glad_INCLUDE_DIR} ${assimp_INCLUDE_DIR}
 ${GTest_INCLUDE_DIR} ${glm_INCLUDE_DIR} ${glfw3_INCLUDE_DIR} ${spdlog_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw fmt::fmt glad::glad assimp::assimp spdlog::spdlog stb::stb)

enable_testing() 
set(TESTS_NAME test_${PROJECT_NAME})
file(GLOB_RECURSE TEST_ARCHIMEDES_SOURCE tests/**.cpp)
add_executable(${TESTS_NAME})
target_compile_definitions(${TESTS_NAME} PUBLIC -DSPDLOG_FMT_EXTERNAL)
target_sources(${TESTS_NAME} PRIVATE ${TEST_ARCHIMEDES_SOURCE})
target_include_directories(${TESTS_NAME} PRIVATE include)
target_link_libraries(${TESTS_NAME} PRIVATE ${PROJECT_NAME} fmt::fmt gtest::gtest GTest::gtest_main spdlog::spdlog stb::stb)
#include(GoogleTest)
#gtest_discover_tests(${TESTS_NAME})

add_executable(${PROJECT_NAME}_bin archimedes_bin/main.cpp)
target_compile_definitions(${PROJECT_NAME}_bin PUBLIC -DSPDLOG_FMT_EXTERNAL)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DSPDLOG_FMT_EXTERNAL)
target_include_directories(${PROJECT_NAME}_bin PUBLIC include)
target_link_libraries(${PROJECT_NAME}_bin PRIVATE ${PROJECT_NAME} spdlog::spdlog fmt::fmt)
