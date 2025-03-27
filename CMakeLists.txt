cmake_minimum_required(VERSION 3.19)

project(
        archimedes
        VERSION 0.1.0
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include("cmake/build_mode.cmake")
include("cmake/os.cmake")
include("cmake/msvc.cmake")
include("cmake/defines.cmake")

# uncomment below to force conan reinstallation
# set(ARCHIMEDES_FORCE_CONAN_INSTALL TRUE)
include("cmake/conan.cmake")

include("cmake/library.cmake")

# CMake targets
file(GLOB_RECURSE ARCHIMEDES_TARGETS cmake/targets/*.cmake)
foreach (ARCHIMEDES_TARGET ${ARCHIMEDES_TARGETS})
    include(${ARCHIMEDES_TARGET})
endforeach ()
