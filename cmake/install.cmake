include_guard()

if(ARCHIMEDES_INSTALL)
	install(
		TARGETS archimedes
		EXPORT archimedesTargets
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin
		INCLUDES DESTINATION include
	)

	install(
		DIRECTORY include/
		DESTINATION include
		FILES_MATCHING
			PATTERN "*.h"
			PATTERN "*.hpp"
	)

	install(
		EXPORT archimedesTargets
		FILE archimedesTargets.cmake
		NAMESPACE archimedes::
		DESTINATION lib/cmake/archimedes
	)

	include(CMakePackageConfigHelpers)

	configure_package_config_file(
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/archimedesConfig.cmake.in
		${CMAKE_CURRENT_BINARY_DIR}/archimedesConfig.cmake
		INSTALL_DESTINATION lib/cmake/archimedes
	)

	install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/archimedesConfig.cmake
		DESTINATION lib/cmake/archimedes
	)
endif()
