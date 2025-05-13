cmake_minimum_required (VERSION 3.16)
project (Archimedes VERSION 0.1 LANGUAGES C CXX)

# CXX STANDARD OPTIONS
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# SET BUILD TYPE TO DEBUG IF NOT SPECIFIED
if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# OPTIONAL TARGETS BUILD
option(ARCHIMEDES_BUILD_TESTS 		"Build Archimedes unit tests" 	OFF)
option(ARCHIMEDES_BUILD_EXAMPLES	"Build Archimedes examples" 	OFF)

# BUILDABLE DEPENDENCIES(GIT SUBMODULES)
add_subdirectory(vendor/cryptopp)
add_subdirectory(vendor/freetype)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/gtest)
add_subdirectory(vendor/jsoncpp)
add_subdirectory(vendor/nvrhi)
add_subdirectory(vendor/openal-soft)
add_subdirectory(vendor/shaderc)
add_subdirectory(vendor/sndfile)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/volk)

# HEADER ONLY LIBRARIES
set(ARCHIMEDES_HEADER_ONLY_INCLUDES 
	${CMAKE_SOURCE_DIR}/vendor/glm
	${CMAKE_SOURCE_DIR}/vendor/stb
	${CMAKE_SOURCE_DIR}/vendor/ctre/include
	${CMAKE_SOURCE_DIR}/vendor/vulkan-headers/include
	${CMAKE_SOURCE_DIR}/vendor/vulkan-memory-allocator/include
)

# ARCHIMEDES STATIC LIB TARGET
file(GLOB_RECURSE ARCHIMEDES_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_library(Archimedes STATIC ${ARCHIMEDES_SRC})
target_compile_features(Archimedes PUBLIC cxx_std_23)

# ARCHIMEDES INCLUDE DIRECTORIES
target_include_directories(Archimedes PUBLIC
	${CMAKE_SOURCE_DIR}/include
	${ARCHIMEDES_HEADER_ONLY_INCLUDES}
)

# LINKING DEPENDENCIES
target_link_libraries(Archimedes PUBLIC
	glfw
	spdlog::spdlog
	OpenAL::OpenAL
	sndfile
	freetype
	jsoncpp
	volk
	shaderc
	nvrhi
	cryptopp
)

# OUTPUT DIRECTORIES]
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ADDING UNIT TESTS (IF ENABLED)
if(ARCHIMEDES_BUILD_EXAMPLES)
	enable_testing()
	add_subdirectory(tests)
endif()

# ADDING EXAMPLES (IF ENABLED)
if(ARCHIMEDES_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()







