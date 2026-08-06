# Configuring archimedes library

include_guard()

add_library(archimedes_lib)

# Find sources for archimedes
file(GLOB_RECURSE ARCHIMEDES_SOURCES CONFIGURE_DEPENDS src/**.cpp)

if(NOT MSVC)
	# Remove pch.cpp from sources
	# msvc needs it, clang & gcc don't use it
	list(REMOVE_ITEM ARCHIMEDES_SOURCES "${PROJECT_SOURCE_DIR}/src/pch.cpp")
endif()

# Add sources to archimedes
target_sources(archimedes_lib PRIVATE ${ARCHIMEDES_SOURCES})

# Add include directories for build and install
target_include_directories(archimedes_lib PUBLIC
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

# Add precompiled headers to archimedes
target_precompile_headers(archimedes_lib PUBLIC "${PROJECT_SOURCE_DIR}/include/archimedes/pch.h")

# Link dependencies
target_link_libraries(archimedes_lib PUBLIC
	nvrhi_vk # idk why, but nvrhi_vk needs to be before nvrhi
	${ARCHIMEDES_LIBRARIES}
	GTest::gmock
)

# Enable IPO
set_property(TARGET archimedes_lib PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

#add_library(archimedes)

#target_sources(archimedes
#	PUBLIC FILE_SET CXX_MODULES
#	FILES "src/archimedes.cppm"
#)
#target_link_libraries(archimedes INTERFACE archimedes_lib)
#target_include_directories(archimedes PUBLIC ${CMAKE_INCLUDE_PATH})

# BuildInfo.h
if(MSVC AND ${ARCHIMEDES_BUILD_TYPE} STREQUAL "Debug")
	set(ARCHIMEDES_NO_INLINING "true")
else()
	set(ARCHIMEDES_NO_INLINING "false")
endif()
set(ARCHIMEDES_BUILDINFO_DIR "${PROJECT_SOURCE_DIR}/build/${ARCHIMEDES_BUILD_TYPE}/include")
file(MAKE_DIRECTORY ${ARCHIMEDES_BUILDINFO_DIR})
configure_file(
	"${CMAKE_SOURCE_DIR}/cmake/BuildInfo.h.in"
	"${ARCHIMEDES_BUILDINFO_DIR}/archimedes/BuildInfo.h"
	@ONLY
)

target_include_directories(archimedes_lib PUBLIC
	$<BUILD_INTERFACE:${ARCHIMEDES_BUILDINFO_DIR}>
)
