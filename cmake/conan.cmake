include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/os.cmake")

# check if conan is installed
execute_process(
	COMMAND "conan" "--version"
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	OUTPUT_VARIABLE CONAN_CHECK_OUT
)
if(NOT CONAN_CHECK_OUT MATCHES ".?Conan.?")
	message(FATAL_ERROR "Conan not found")
else()
	message(STATUS "Conan present")
endif()

# check for conan files
if(ARCHIMEDES_FORCE_CONAN_INSTALL OR NOT EXISTS "${PROJECT_SOURCE_DIR}/cmake/conan_files/${ARCHIMEDES_BUILD_TYPE}/conan_toolchain.cmake")
	if(ARCHIMEDES_FORCE_CONAN_INSTALL)
		message(STATUS "Forced Conan configuration for ${ARCHIMEDES_BUILD_TYPE} mode")
	else()
		message(STATUS "Conan files not found for ${ARCHIMEDES_BUILD_TYPE} mode, configuring conan for ${ARCHIMEDES_BUILD_TYPE} mode")
	endif()

	execute_process(
		COMMAND conan profile detect --force
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_QUIET
        ERROR_QUIET
	) 

	file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/cmake/conan_files/${ARCHIMEDES_BUILD_TYPE}/")

	# option only for MSCV
	if(MSVC)
		set(ARCHIMEDES_CONAN_COMPILER_RUNTIME_TYPE "-s:b compiler.runtime_type=${ARCHIMEDES_BUILD_TYPE} -s:h compiler.runtime_type=${ARCHIMEDES_BUILD_TYPE}")
	endif()

	if(${ARCHIMEDES_LINUX})
		set(ARCHIMEDES_CONAN_SYSTEM_PACKAGE_MANAGER "-c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True")
	endif()

	# install conan requirements
	execute_process(
		COMMAND conan install . -s:b build_type=${ARCHIMEDES_BUILD_TYPE} -s:b compiler.cppstd=20 -s:h build_type=${ARCHIMEDES_BUILD_TYPE} -s:h compiler.cppstd=20 ${ARCHIMEDES_CONAN_INSTALL_RUNTIME_TYPE} ${ARCHIMEDES_CONAN_SYSTEM_PACKAGE_MANAGER} -of=cmake/conan_files/${ARCHIMEDES_BUILD_TYPE} --build=missing
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE ARCHIMEDES_CONAN_INSTALL_OUTPUT
		RESULT_VARIABLE ARCHIMEDES_CONAN_INSTALL_RESULT
	)

	# check conan result
	if(NOT ${ARCHIMEDES_CONAN_INSTALL_RESULT} EQUAL 0)
		file(REMOVE_RECURSE "${PROJECT_SOURCE_DIR}/cmake/conan_files/${ARCHIMEDES_BUILD_TYPE}/")
		message(FATAL_ERROR "Conan installation failed")
	else()
		message(STATUS "Conan installation succeded")
	endif()
else()
	message(STATUS "Conan files found for ${ARCHIMEDES_BUILD_TYPE} mode") 
endif()

# manual include conan toolchain
include("${PROJECT_SOURCE_DIR}/cmake/conan_files/${ARCHIMEDES_BUILD_TYPE}/conan_toolchain.cmake")
list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/conan_files/${ARCHIMEDES_BUILD_TYPE})

# includes from conan
include_directories(${CMAKE_INCLUDE_PATH})

# automatic find_package()
include("${PROJECT_SOURCE_DIR}/cmake/conan_files/${ARCHIMEDES_BUILD_TYPE}/conandeps_legacy.cmake")
list(APPEND ARCHIMEDES_LIBRARIES ${CONANDEPS_LEGACY})
