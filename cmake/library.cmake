include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/conan.cmake")

add_library(${PROJECT_NAME})
target_precompile_headers(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include/archimedes/pch.h")

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
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include> )

# link conan libraries
target_link_libraries(${PROJECT_NAME} PUBLIC nvrhi_vk ${ARCHIMEDES_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)


#include("${PROJECT_SOURCE_DIR}/cmake/non_conan_deps.cmake")
