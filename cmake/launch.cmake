# Setup launch targets for Visual Studio or CLion

include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/launch/visual_studio.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/launch/clion.cmake")

# Generic
function(LaunchAddTarget EXE_TARGET)
	LaunchVisualStudioAddTarget(${EXE_TARGET})
	LaunchCLionAddTarget(${EXE_TARGET})
endfunction()

# Generic
function(LaunchMake)
	LaunchVisualStudioMake()
	LaunchCLionMake()
endfunction()