include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/launch/visual_studio.cmake")

function(LaunchAddTarget EXE_TARGET)
	LaunchVisualStudioAddTarget(${EXE_TARGET})
endfunction()

function(LaunchMake)
	LaunchVisualStudioMake()
endfunction()