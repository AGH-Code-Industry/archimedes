include_guard()

# adds interface for automatically setting run as working directory in Visual Studio

if(EXISTS "${CMAKE_SOURCE_DIR}/.vs" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/.vs")
	# make global targets list
	if(NOT DEFINED LaunchVSJSON_LIST)
		set(LaunchVSJSON_LIST "" CACHE INTERNAL "")
	endif()

	# adds EXE_TARGET to targets list
	function(LaunchVSJSONAddTarget EXE_TARGET)
		get_property(CURR GLOBAL PROPERTY LaunchVSJSON_LIST)
		list(APPEND CURR "${EXE_TARGET}")
		set_property(GLOBAL PROPERTY LaunchVSJSON_LIST "${CURR}")
	endfunction()

	# creates .vs/launch.vs.json from targets list
	function(LaunchVSJSONMake)
		get_property(CURR GLOBAL PROPERTY LaunchVSJSON_LIST)
		set(LaunchVSJSON_PATH "${CMAKE_SOURCE_DIR}/.vs/launch.vs.json")
		file(WRITE ${LaunchVSJSON_PATH} "")
		set(LaunchVSJSON_STR 
"{
	\"version\": \"0.2.1\",
	\"defaults\": {},
	\"configurations\": ["
		)

		foreach(EXE_TARGET ${CURR})
			string(APPEND LaunchVSJSON_STR 
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
		string(LENGTH ${LaunchVSJSON_STR} LEN)
		math(EXPR LEN "${LEN}-1")
		string(SUBSTRING ${LaunchVSJSON_STR} 0 ${LEN} LaunchVSJSON_STR)

		string(APPEND LaunchVSJSON_STR 
"
	]
}
"
		)
		file(WRITE ${LaunchVSJSON_PATH} ${LaunchVSJSON_STR})
	endfunction()
else()
	# empty variants if not VS

	function(LaunchVSJSONAddTarget EXE_TARGET)
	endfunction()

	function(LaunchVSJSONMake)
	endfunction()
endif()

