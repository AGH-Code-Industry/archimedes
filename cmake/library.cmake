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

# install msdf-atlas-gen/1.3 Release
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/cmake/conan_files/msdf_atlas_gen_1_3_installed")
    message(STATUS "msdf-atlas-gen not installed, installing...")
    execute_process(
        COMMAND conan install --requires=msdf-atlas-gen/1.3 --build=missing
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    file(WRITE "${PROJECT_SOURCE_DIR}/cmake/conan_files/msdf_atlas_gen_1_3_installed" "")
else()
    message(STATUS "msdf-atlas-gen installed")
endif()
