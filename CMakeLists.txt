# Main CMake file

cmake_minimum_required(VERSION 3.19)

if(${CMAKE_MAJOR_VERSION} GREATER 3)
	message(FATAL_ERROR "CMake >=4.X.X is not supported")
endif()

project(
        archimedes
        VERSION 2025.08.24
        LANGUAGES CXX C
)

# C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Add MSVC-specific behavior
include("cmake/msvc.cmake")

# Add OS handling
include("cmake/os.cmake")

# Add CMake options
include("cmake/options.cmake")

# Add build_mode handling
include("cmake/build_mode.cmake")

# uncomment below to force conan reinstallation
#set(ARCHIMEDES_FORCE_CONAN_INSTALL TRUE)

# Add conan handling
include("cmake/conan.cmake")

# Add Ninja fixes
include("cmake/ninja_fix.cmake")

# Add library behavior
include("cmake/library.cmake")

# Add installation behavior
include("cmake/install.cmake")

# Add lauch settings creation
include("cmake/launch.cmake")

# Add macro defines
include("cmake/defines.cmake")

# CMake targets
include("cmake/targets.cmake")
