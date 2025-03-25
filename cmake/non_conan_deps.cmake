include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/library.cmake")
include(FetchContent)

function(GitDepDownload PackageName Git GitTag)
    FetchContent_Declare(
            ${PackageName}
            GIT_REPOSITORY ${Git}
            GIT_TAG ${GitTag}
            OVERRIDE_FIND_PACKAGE
    )
    if(EXISTS "${FETCHCONTENT_BASE_DIR}/${PackageName}_downloaded")
        message(STATUS "${PackageName} found")
        FetchContent_Populate(
            ${PackageName}
            QUIET
            SUBBUILD_DIR "${FETCHCONTENT_BASE_DIR}/${PackageName}-subbuild"
            SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/${PackageName}-src"
            BINARY_DIR "${FETCHCONTENT_BASE_DIR}/${PackageName}-build"
        )
        add_subdirectory("${FETCHCONTENT_BASE_DIR}/${PackageName}-src" "${FETCHCONTENT_BASE_DIR}/${PackageName}-build")
    else()
        message(STATUS "${PackageName} not found, downloading...")
        # find_package triggers download
        find_package(${PackageName} REQUIRED)
        write_file("${FETCHCONTENT_BASE_DIR}/${PackageName}_downloaded" "")
        message(STATUS "${PackageName} downloaded")
    endif()
endfunction()

GitDepDownload(nvrhi https://github.com/NVIDIAGameWorks/NvRhi.git main)
target_link_libraries(${PROJECT_NAME} PRIVATE nvrhi_vk nvrhi)