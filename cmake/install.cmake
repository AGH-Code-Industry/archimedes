# Package install
include_guard()

if(ARCHIMEDES_INSTALL)

    # Install the library target
    install(
        TARGETS archimedes_lib
        EXPORT archimedes_libTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        FILE_SET CXX_MODULES DESTINATION include/modules
    )

    # Install public headers
    install(
        DIRECTORY include/
        DESTINATION include
        FILES_MATCHING
            PATTERN "*.h"
            PATTERN "*.hpp"
    )

    # Install module interface units (.cppm)
    install(
        DIRECTORY src/
        DESTINATION include/modules
        FILES_MATCHING
            PATTERN "*.cppm"
    )

    # Install exported targets and generated C++ module metadata
    install(
        EXPORT archimedes_libTargets
        FILE archimedes_libTargets.cmake
        NAMESPACE archimedes_lib::
        DESTINATION lib/cmake/archimedes_lib
        CXX_MODULES_DIRECTORY include/modules
    )

    # Generate the package configuration file
    include(CMakePackageConfigHelpers)

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/archimedes_libConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/archimedes_libConfig.cmake
        INSTALL_DESTINATION lib/cmake/archimedes_lib
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/archimedes_libConfig.cmake
        DESTINATION lib/cmake/archimedes_lib
    )

endif()