include_guard()

# adds interface for automatically setting run as working directory in CLion

if(EXISTS "${CMAKE_SOURCE_DIR}/.idea" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/.idea")
	# make global targets list
	if(NOT DEFINED LAUNCH_CLION_LIST)
		set(LAUNCH_CLION_LIST "" CACHE INTERNAL "")
	endif()

	# adds EXE_TARGET to targets list
	function(LaunchCLionAddTarget EXE_TARGET)
		get_property(CURR GLOBAL PROPERTY LAUNCH_CLION_LIST)
		list(APPEND CURR "${EXE_TARGET}")
		set_property(GLOBAL PROPERTY LAUNCH_CLION_LIST "${CURR}")
	endfunction()

	# creates .vs/launch.vs.json from targets list
	function(LaunchCLionMake)
		get_property(TARGETS_LIST GLOBAL PROPERTY LAUNCH_CLION_LIST)

		file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/.run")

		foreach(EXE_TARGET ${TARGETS_LIST})
			file(WRITE "${CMAKE_SOURCE_DIR}/.run/${EXE_TARGET}.run.xml"
"<component name=\"ProjectRunConfigurationManager\">
  <configuration default=\"false\" name=\"${EXE_TARGET}\" type=\"CMakeRunConfiguration\" factoryName=\"Application\" REDIRECT_INPUT=\"false\" ELEVATE=\"false\" USE_EXTERNAL_CONSOLE=\"false\" EMULATE_TERMINAL=\"false\" WORKING_DIR=\"file://${CMAKE_SOURCE_DIR}/run\" PASS_PARENT_ENVS_2=\"true\" PROJECT_NAME=\"archimedes\" TARGET_NAME=\"${EXE_TARGET}\" RUN_TARGET_PROJECT_NAME=\"archimedes\" RUN_TARGET_NAME=\"${EXE_TARGET}\">
    <method v=\"2\">
      <option name=\"com.jetbrains.cidr.execution.CidrBuildBeforeRunTaskProvider$BuildBeforeRunTask\" enabled=\"true\" />
    </method>
  </configuration>
</component>
"
			)
		endforeach()
	endfunction()
else()
	# empty variants if not CLion

	function(LaunchCLionAddTarget EXE_TARGET)
	endfunction()

	function(LaunchCLionMake)
	endfunction()
endif()

