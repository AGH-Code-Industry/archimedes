# Package install

include_guard()

if(ARCHIMEDES_INSTALL)
	# Install archimedes
	install(
		TARGETS archimedes
		EXPORT archimedesTargets
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin
		INCLUDES DESTINATION include
	)

	# Add headers
	install(
		DIRECTORY include/
		DESTINATION include
		FILES_MATCHING
			PATTERN "*.h"
			PATTERN "*.hpp"
	)

	# Add targets
	install(
		EXPORT archimedesTargets
		FILE archimedesTargets.cmake
		NAMESPACE archimedes::
		DESTINATION lib/cmake/archimedes
	)

	# Use template file (.cmake.in)
	include(CMakePackageConfigHelpers)
	configure_package_config_file(
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/archimedesConfig.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/archimedesConfig.cmake
		INSTALL_DESTINATION lib/cmake/archimedes
	)

	# Use config file
	install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/archimedesConfig.cmake
		DESTINATION lib/cmake/archimedes
	)
endif()
