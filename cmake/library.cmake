include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/conan.cmake")

add_library(${PROJECT_NAME})

# find source files
file(GLOB_RECURSE ARCHIMEDES_SOURCE CONFIGURE_DEPENDS src/**.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${ARCHIMEDES_SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# link conan libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${ARCHIMEDES_LIBRARIES})


include(FetchContent)
set(NVRHI_WITH_VULKAN TRUE)
FetchContent_Declare(
        nvrhi
        GIT_REPOSITORY https://github.com/NVIDIAGameWorks/NvRhi.git
        GIT_TAG main
        OVERRIDE_FIND_PACKAGE
)
FETCHCONTENT_MAKEAVAILABLE(nvrhi)
find_package(nvrhi REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC nvrhi nvrhi_vk)