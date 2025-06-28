include_guard()

# adds interface for automatically setting run as working directory in Visual Studio

if(EXISTS "${CMAKE_SOURCE_DIR}/.vs" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/.vs")
	# make global targets list
	if(NOT DEFINED LAUNCH_VISUAL_STUDIO_LIST)
		set(LAUNCH_VISUAL_STUDIO_LIST "" CACHE INTERNAL "")
	endif()

	# adds EXE_TARGET to targets list
	function(LaunchVisualStudioAddTarget EXE_TARGET)
		get_property(CURR GLOBAL PROPERTY LAUNCH_VISUAL_STUDIO_LIST)
		list(APPEND CURR "${EXE_TARGET}")
		set_property(GLOBAL PROPERTY LAUNCH_VISUAL_STUDIO_LIST "${CURR}")
	endfunction()

	# creates .vs/launch.vs.json from targets list
	function(LaunchVisualStudioMake)
		get_property(TARGETS_LIST GLOBAL PROPERTY LAUNCH_VISUAL_STUDIO_LIST)
		set(LAUNCH_VISUAL_STUDIO_PATH "${CMAKE_SOURCE_DIR}/.vs/launch.vs.json")
		file(WRITE ${LAUNCH_VISUAL_STUDIO_PATH} "")
		set(LAUNCH_VISUAL_STUDIO_STR 
"{
	\"version\": \"0.2.1\",
	\"defaults\": {},
	\"configurations\": ["
		)

		foreach(EXE_TARGET ${TARGETS_LIST})
			string(APPEND LAUNCH_VISUAL_STUDIO_STR 
"
		{
			\"type\": \"default\",
			\"project\": \"CMakeLists.txt\",
			\"projectTarget\": \"${EXE_TARGET}.exe\",
			\"name\": \"${EXE_TARGET}.exe\",
			\"currentDir\": \"\${workspaceRoot}/run\"
		},"
			)
		endforeach()

		# remove comma
		string(LENGTH ${LAUNCH_VISUAL_STUDIO_STR} LEN)
		math(EXPR LEN "${LEN}-1")
		string(SUBSTRING ${LAUNCH_VISUAL_STUDIO_STR} 0 ${LEN} LAUNCH_VISUAL_STUDIO_STR)

		string(APPEND LAUNCH_VISUAL_STUDIO_STR 
"
	]
}
"
		)
		file(WRITE ${LAUNCH_VISUAL_STUDIO_PATH} ${LAUNCH_VISUAL_STUDIO_STR})
	endfunction()
else()
	# empty variants if not VS

	function(LaunchVisualStudioAddTarget EXE_TARGET)
	endfunction()

	function(LaunchVisualStudioMake)
	endfunction()
endif()

