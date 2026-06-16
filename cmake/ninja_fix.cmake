# Fixes for Ninja generator

include_guard()

# Ninja breaks when conan settings.arch is set
if(CMAKE_GENERATOR MATCHES "Ninja")
	unset(CMAKE_GENERATOR_PLATFORM CACHE)
	unset(CMAKE_GENERATOR_TOOLSET CACHE)
endif()