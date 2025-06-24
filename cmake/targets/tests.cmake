include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/library.cmake")

set(ARCHIMEDES_TEST "${PROJECT_NAME}_tests")
add_executable(${ARCHIMEDES_TEST})
file(GLOB_RECURSE ARCHIMEDES_TEST_SOURCE tests/**.cpp)
target_sources(${ARCHIMEDES_TEST} PUBLIC ${ARCHIMEDES_TEST_SOURCE})
target_link_libraries(${ARCHIMEDES_TEST} PUBLIC ${PROJECT_NAME})

LaunchVSJSONAddTarget(${ARCHIMEDES_TEST})