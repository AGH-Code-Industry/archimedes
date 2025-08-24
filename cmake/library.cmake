# Configuring archimedes library

include_guard()

add_library(archimedes)

# Find sources for archimedes
file(GLOB_RECURSE ARCHIMEDES_SOURCES CONFIGURE_DEPENDS src/**.cpp)

if(NOT MSVC)
	# Remove pch.cpp from sources
	# msvc needs it, clang & gcc don't use it
	list(REMOVE_ITEM ARCHIMEDES_SOURCES "${PROJECT_SOURCE_DIR}/src/pch.cpp")
endif()

# Add sources to archimedes
target_sources(archimedes PRIVATE ${ARCHIMEDES_SOURCES})

# Add include directories for build and install
target_include_directories(archimedes PUBLIC
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

# Add precompiled headers to archimedes
target_precompile_headers(archimedes PUBLIC "${PROJECT_SOURCE_DIR}/include/archimedes/pch.h")

# Link dependencies
target_link_libraries(archimedes PUBLIC
	nvrhi_vk # idk why, but nvrhi_vk needs to be before nvrhi
	${ARCHIMEDES_LIBRARIES}
)

# Enable IPO
set_property(TARGET archimedes PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
