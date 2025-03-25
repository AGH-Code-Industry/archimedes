include_guard()

# check build type
if(
    "${CMAKE_CONFIGURATION_TYPES}" STREQUAL Debug OR
    "${CMAKE_BUILD_TYPE}" STREQUAL Debug
)
    set(ARCHIMEDES_DEBUG TRUE)
    set(ARCHIMEDES_RELEASE FALSE)

    set(ARCHIMEDES_BUILD_TYPE "Debug")
    
    add_compile_definitions(ARCHIMEDES_DEBUG=1)
    add_compile_definitions(ARCHIMEDES_RELEASE=0)
elseif(
    "${CMAKE_CONFIGURATION_TYPES}" STREQUAL "RelWithDebInfo" OR
    "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo"
)
    set(ARCHIMEDES_DEBUG FALSE)
    set(ARCHIMEDES_RELEASE TRUE)
    
    set(ARCHIMEDES_BUILD_TYPE "RelWithDebInfo")
    
    add_compile_definitions(ARCHIMEDES_DEBUG=0)
    add_compile_definitions(ARCHIMEDES_RELEASE=1)
elseif(
    "${CMAKE_CONFIGURATION_TYPES}" STREQUAL "Release" OR
    "${CMAKE_BUILD_TYPE}" STREQUAL "Release"
)
    set(ARCHIMEDES_DEBUG FALSE)
    set(ARCHIMEDES_RELEASE TRUE)
    
    set(ARCHIMEDES_BUILD_TYPE "Release")
    
    add_compile_definitions(ARCHIMEDES_DEBUG=0)
    add_compile_definitions(ARCHIMEDES_RELEASE=1)
endif()

message(STATUS "Build mode: ${ARCHIMEDES_BUILD_TYPE}")

set(CMAKE_BUILD_TYPE ${ARCHIMEDES_BUILD_TYPE})
