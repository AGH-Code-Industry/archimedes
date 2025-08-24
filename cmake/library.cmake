include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/conan.cmake")

add_library(${PROJECT_NAME})
target_precompile_headers(${PROJECT_NAME} PUBLIC "include/archimedes/pch.h")

# find source files
file(GLOB_RECURSE ARCHIMEDES_SOURCE CONFIGURE_DEPENDS src/**.cpp src/platform/**.h)

# remove pch.cpp
# msvc needs it
# gcc/clang don't use it
if(NOT MSVC)
	file(GLOB_RECURSE ARCHIMEDES_PCH_SOURCES CONFIGURE_DEPENDS src/pch.cpp)
	list(GET ARCHIMEDES_PCH_SOURCES 0 ARCHIMEDES_PCH_SOURCE)
	list(REMOVE_ITEM ARCHIMEDES_SOURCE ${ARCHIMEDES_PCH_SOURCE})
endif()

target_sources(${PROJECT_NAME} PRIVATE ${ARCHIMEDES_SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC "include/archimedes")

# link conan libraries
target_link_libraries(${PROJECT_NAME} PUBLIC nvrhi_vk ${ARCHIMEDES_LIBRARIES})

get_target_property(LINKED_LIBS ${PROJECT_NAME} LINK_LIBRARIES)
message(STATUS "Libraries linked to ${PROJECT_NAME}: ${LINKED_LIBS}")

#include("${PROJECT_SOURCE_DIR}/cmake/non_conan_deps.cmake")
