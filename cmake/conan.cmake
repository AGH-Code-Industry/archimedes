# Conan handling

include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/os.cmake")

set(ARCHIMEDES_CONAN_FILES "${PROJECT_SOURCE_DIR}/conan_files/${ARCHIMEDES_BUILD_TYPE}/build/generators")

set(ARCHIMEDES_CONANFILE "${PROJECT_SOURCE_DIR}/conanfile_dev.py")
set(ARCHIMEDES_CONANFILE_HASH "${ARCHIMEDES_CONAN_FILES}/conanfile.hash")

# Check for changes in conanfile
file(SHA256 "${ARCHIMEDES_CONANFILE}" ARCHIMEDES_CONANFILE_HASH_NEW_VALUE) # compute hash
if(EXISTS ${ARCHIMEDES_CONANFILE_HASH})
	set(ARCHIMEDES_CONAN_INSTALLED TRUE)

	# get hash value from file
	file(STRINGS ${ARCHIMEDES_CONANFILE_HASH} ARCHIMEDES_CONAN_INSTALL_HASH_CONTENTS LIMIT_COUNT 1)
	list(GET ARCHIMEDES_CONAN_INSTALL_HASH_CONTENTS 0 ARCHIMEDES_CONANFILE_HASH_OLD_VALUE)

	if(${ARCHIMEDES_CONANFILE_HASH_OLD_VALUE} STREQUAL ${ARCHIMEDES_CONANFILE_HASH_NEW_VALUE})
		set(ARCHIMEDES_CONANFILE_HASH_EQ TRUE)
		set(ARCHIMEDES_CONAN_INSTALL FALSE)
	else()
		set(ARCHIMEDES_CONANFILE_HASH_EQ FALSE)
		set(ARCHIMEDES_CONAN_INSTALL TRUE)
	endif()
else() # conanfile was not installed
	set(ARCHIMEDES_CONAN_INSTALLED FALSE)
	set(ARCHIMEDES_CONAN_INSTALL TRUE)
endif()
if(ARCHIMEDES_FORCE_CONAN_INSTALL)
	set(ARCHIMEDES_CONAN_INSTALL TRUE)
endif()

message("Conan installation status:")
if(ARCHIMEDES_CONAN_INSTALL)
	if(ARCHIMEDES_FORCE_CONAN_INSTALL)
		message("    forced")
	elseif(NOT ARCHIMEDES_CONANFILE_HASH_EQ)
		message("    conanfile changed")
	elseif(NOT ARCHIMEDES_CONAN_INSTALLED)
		message("    not installed")
	endif()

	# check for conan
	execute_process(COMMAND
		conan version
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE ARCHIMEDES_CONAN_OUTPUT
	)
	# match with expected output
	if(NOT ARCHIMEDES_CONAN_OUTPUT MATCHES "^version:[^\n]*\nconan_path:[^\n]*")
		message(FATAL_ERROR "Conan not found")
	else()
		message(STATUS "Conan found")
	endif()

	# set environment variables
	set(ENV{CC} ${CMAKE_C_COMPILER})
	set(ENV{CXX} ${CMAKE_CXX_COMPILER})

	# detect conan profile
	execute_process(
		COMMAND conan profile detect --force
		COMMAND_ERROR_IS_FATAL ANY
	)

	file(MAKE_DIRECTORY ${ARCHIMEDES_CONAN_FILES})

	# MSVC-only setting
	if(MSVC)
		set(ARCHIMEDES_CONAN_COMPILER_RUNTIME_TYPE "-s:b compiler.runtime_type=${CMAKE_BUILD_TYPE} -s:h compiler.runtime_type=${CMAKE_BUILD_TYPE}")
	endif()

	# Set package manager settings for Linux
	if(${ARCHIMEDES_LINUX})
		set(ARCHIMEDES_CONAN_TOOLS_SYSTEM_PACKAGE_MANAGER_MODE "-c tools.system.package_manager:mode=install")
		set(ARCHIMEDES_CONAN_TOOLS_SYSTEM_PACKAGE_MANAGER_SUDO "-c tools.system.package_manager:sudo=True")
	endif()

	# Parse additional conan args
	separate_arguments(ARCHIMEDES_CONAN_ARGS_LIST UNIX_COMMAND "${ARCHIMEDES_CONAN_ARGS}")

	# run the install command
	execute_process(
		COMMAND conan install ./conanfile_dev.py -s:b build_type=${ARCHIMEDES_BUILD_TYPE} -s:b compiler.cppstd=${CMAKE_CXX_STANDARD} -s:h build_type=${ARCHIMEDES_BUILD_TYPE} -s:h compiler.cppstd=${CMAKE_CXX_STANDARD} ${ARCHIMEDES_CONAN_INSTALL_RUNTIME_TYPE} ${ARCHIMEDES_CONAN_TOOLS_SYSTEM_PACKAGE_MANAGER_SUDO} ${ARCHIMEDES_CONAN_TOOLS_SYSTEM_PACKAGE_MANAGER_MODE} -of=conan_files/${ARCHIMEDES_BUILD_TYPE} --build=missing -pr default
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		OUTPUT_VARIABLE ARCHIEMDES_CONAN_INSTALL_OUTPUT
		RESULT_VARIABLE ARCHIEMDES_CONAN_INSTALL_RESULT
		COMMAND_ECHO STDOUT
	)

	# check installation results
	if(NOT ${ARCHIEMDES_CONAN_INSTALL_RESULT} EQUAL 0)
		file(REMOVE_RECURSE ${ARCHIMEDES_CONAN_FILES})
		message(FATAL_ERROR "Conan installation failed")
	else()
		file(WRITE ${ARCHIMEDES_CONANFILE_HASH} ${ARCHIMEDES_CONANFILE_HASH_NEW_VALUE})
		message("Conan installation completed")
	endif()
else()
	message("\tfound files for ${CMAKE_BUILD_TYPE}")
endif()

# find & include toolchain & conandeps
file(GLOB_RECURSE ARCHIMEDES_TOOLCHAIN_CONANDEPS "${PROJECT_SOURCE_DIR}/conan_files/${ARCHIMEDES_BUILD_TYPE}/*/conan_toolchain.cmake" "${PROJECT_SOURCE_DIR}/conan_files/${ARCHIMEDES_BUILD_TYPE}/*/conandeps_legacy.cmake")
foreach(ARCHIMEDES_TOOLCHAIN_CONANDEPS_FILE ${ARCHIMEDES_TOOLCHAIN_CONANDEPS})
	include(${ARCHIMEDES_TOOLCHAIN_CONANDEPS_FILE})
endforeach()

# Add conan libraries to archimedes
list(APPEND ARCHIMEDES_LIBRARIES ${CONANDEPS_LEGACY})
