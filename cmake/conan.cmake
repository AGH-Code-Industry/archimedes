include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/os.cmake")

SET(ARCHIMEDES_CONAN_TOOLCHAIN_PATH "${PROJECT_SOURCE_DIR}/cmake/conan_files/${ARCHIMEDES_BUILD_TYPE}/conan_toolchain.cmake")
SET(ARCHIMEDES_CONAN_INSTALL_HASH_PATH "${PROJECT_SOURCE_DIR}/cmake/conan_files/${ARCHIMEDES_BUILD_TYPE}/conanfile.hash")

file(SHA256 "${PROJECT_SOURCE_DIR}/conanfile.py" ARCHIMEDES_CONANFILE_HASH)
if(EXISTS ${ARCHIMEDES_CONAN_INSTALL_HASH_PATH})
	file(STRINGS ${ARCHIMEDES_CONAN_INSTALL_HASH_PATH} ARCHIMEDES_CONAN_INSTALL_HASH_LIST LIMIT_COUNT 1)
	list(GET ARCHIMEDES_CONAN_INSTALL_HASH_LIST 0 ARCHIMEDES_CONAN_INSTALL_HASH)

	if(${ARCHIMEDES_CONAN_INSTALL_HASH} STREQUAL ${ARCHIMEDES_CONANFILE_HASH})
		SET(ARCHIMEDES_CONAN_INSTALL_HASH_NEQ FALSE)
	else()
		SET(ARCHIMEDES_CONAN_INSTALL_HASH_NEQ TRUE)
	endif()
endif()

# check for conan files
if(ARCHIMEDES_FORCE_CONAN_INSTALL OR ARCHIMEDES_CONAN_INSTALL_HASH_NEQ OR NOT EXISTS ${ARCHIMEDES_CONAN_TOOLCHAIN_PATH} OR NOT EXISTS ${ARCHIMEDES_CONAN_INSTALL_HASH_PATH})
	if(ARCHIMEDES_FORCE_CONAN_INSTALL)
		message(STATUS "Forced Conan configuration for ${ARCHIMEDES_BUILD_TYPE} mode")
	elseif(NOT EXISTS ${ARCHIMEDES_CONAN_TOOLCHAIN_PATH} OR NOT EXISTS ${ARCHIMEDES_CONAN_INSTALL_HASH_PATH})
		message(STATUS "Conan files not found for ${ARCHIMEDES_BUILD_TYPE} mode, configuring conan for ${ARCHIMEDES_BUILD_TYPE} mode")
	elseif(ARCHIMEDES_CONAN_INSTALL_HASH_NEQ)
		message(STATUS "conanfile.py was edited, configuring conan for ${ARCHIMEDES_BUILD_TYPE} mode")
	endif()

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

	set(ENV{CC} ${CMAKE_C_COMPILER})
	set(ENV{CXX} ${CMAKE_CXX_COMPILER})
	execute_process(
		COMMAND "conan" "profile" "detect" "--force"
		COMMAND_ERROR_IS_FATAL ANY
	)

	file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/cmake/conan_files/${ARCHIMEDES_BUILD_TYPE}/")

	# option only for MSCV
	if(MSVC)
		set(ARCHIMEDES_CONAN_COMPILER_RUNTIME_TYPE "-s:b compiler.runtime_type=${ARCHIMEDES_BUILD_TYPE} -s:h compiler.runtime_type=${ARCHIMEDES_BUILD_TYPE}")
	endif()

	if(${ARCHIMEDES_LINUX})
		set(ARCHIMEDES_CONAN_SYSTEM_PACKAGE_MANAGER_MODE "-c tools.system.package_manager:mode=install")
		set(ARCHIMEDES_CONAN_SYSTEM_PACKAGE_MANAGER_SUDO "-c tools.system.package_manager:sudo=True")
	endif()

	separate_arguments(CONAN_ARGS_LIST UNIX_COMMAND "${ARCHIMEDES_CONAN_ARGS}")
	# install conan requirements
	execute_process(
		COMMAND conan install . -s:b build_type=${ARCHIMEDES_BUILD_TYPE} -s:b compiler.cppstd=${CMAKE_CXX_STANDARD} -s:h build_type=${ARCHIMEDES_BUILD_TYPE} -s:h compiler.cppstd=${CMAKE_CXX_STANDARD} ${ARCHIMEDES_CONAN_INSTALL_RUNTIME_TYPE} ${ARCHIMEDES_CONAN_SYSTEM_PACKAGE_MANAGER_SUDO} ${ARCHIMEDES_CONAN_SYSTEM_PACKAGE_MANAGER_MODE} -of=cmake/conan_files/${ARCHIMEDES_BUILD_TYPE} --build=missing -pr default ${CONAN_ARGS_LIST}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE ARCHIMEDES_CONAN_INSTALL_OUTPUT
		RESULT_VARIABLE ARCHIMEDES_CONAN_INSTALL_RESULT
	)

	# check conan result
	if(NOT ${ARCHIMEDES_CONAN_INSTALL_RESULT} EQUAL 0)
		file(REMOVE_RECURSE "${PROJECT_SOURCE_DIR}/cmake/conan_files/${ARCHIMEDES_BUILD_TYPE}/")
		message(FATAL_ERROR "Conan installation failed")
	else()
		file(WRITE ${ARCHIMEDES_CONAN_INSTALL_HASH_PATH} ${ARCHIMEDES_CONANFILE_HASH})

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
