include_guard()

# adds interface for automatically setting run as working directory in Visual Studio

if(EXISTS "${CMAKE_SOURCE_DIR}/.vs" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/.vs")
	# make global targets list
	if(NOT DEFINED LaunchVisualStudio_LIST)
		set(LaunchVisualStudio_LIST "" CACHE INTERNAL "")
	endif()

	# adds EXE_TARGET to targets list
	function(LaunchVisualStudioAddTarget EXE_TARGET)
		get_property(CURR GLOBAL PROPERTY LaunchVisualStudio_LIST)
		list(APPEND CURR "${EXE_TARGET}")
		set_property(GLOBAL PROPERTY LaunchVisualStudio_LIST "${CURR}")
	endfunction()

	# creates .vs/launch.vs.json from targets list
	function(LaunchVisualStudioMake)
		get_property(CURR GLOBAL PROPERTY LaunchVisualStudio_LIST)
		set(LaunchVisualStudio_PATH "${CMAKE_SOURCE_DIR}/.vs/launch.vs.json")
		file(WRITE ${LaunchVisualStudio_PATH} "")
		set(LaunchVisualStudio_STR 
"{
	\"version\": \"0.2.1\",
	\"defaults\": {},
	\"configurations\": ["
		)

		foreach(EXE_TARGET ${CURR})
			string(APPEND LaunchVisualStudio_STR 
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
		string(LENGTH ${LaunchVisualStudio_STR} LEN)
		math(EXPR LEN "${LEN}-1")
		string(SUBSTRING ${LaunchVisualStudio_STR} 0 ${LEN} LaunchVisualStudio_STR)

		string(APPEND LaunchVisualStudio_STR 
"
	]
}
"
		)
		file(WRITE ${LaunchVisualStudio_PATH} ${LaunchVisualStudio_STR})
	endfunction()
else()
	# empty variants if not VS

	function(LaunchVisualStudioAddTarget EXE_TARGET)
	endfunction()

	function(LaunchVisualStudioMake)
	endfunction()
endif()

