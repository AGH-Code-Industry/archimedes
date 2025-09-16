# Build type handling

include_guard()

# Check build type
if(
    "${CMAKE_CONFIGURATION_TYPES}" STREQUAL Debug OR
    "${CMAKE_BUILD_TYPE}" STREQUAL Debug
)
    set(ARCHIMEDES_DEBUG TRUE)
    set(ARCHIMEDES_RELEASE FALSE)

    set(ARCHIMEDES_BUILD_TYPE "Debug")
elseif(
    "${CMAKE_CONFIGURATION_TYPES}" STREQUAL "RelWithDebInfo" OR
    "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo"
)
    set(ARCHIMEDES_DEBUG FALSE)
    set(ARCHIMEDES_RELEASE TRUE)
    
    set(ARCHIMEDES_BUILD_TYPE "RelWithDebInfo")
else(
	# Release or MinSizeRel
)  
    set(ARCHIMEDES_DEBUG FALSE)
    set(ARCHIMEDES_RELEASE TRUE)
    
    set(ARCHIMEDES_BUILD_TYPE "Release")
endif()

message(STATUS "Build mode: ${ARCHIMEDES_BUILD_TYPE}")

set(CMAKE_BUILD_TYPE ${ARCHIMEDES_BUILD_TYPE})
