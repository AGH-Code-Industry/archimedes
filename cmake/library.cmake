include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/conan.cmake")

add_library(${PROJECT_NAME})

# find source files
file(GLOB_RECURSE ARCHIMEDES_SOURCE CONFIGURE_DEPENDS src/**.cpp src/platform/**.h)
target_sources(${PROJECT_NAME} PRIVATE ${ARCHIMEDES_SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# link conan libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${ARCHIMEDES_LIBRARIES})

include("${PROJECT_SOURCE_DIR}/cmake/non_conan_deps.cmake")

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
