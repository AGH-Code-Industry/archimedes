include_guard()

# module library
add_library(archimedes)

file(GLOB_RECURSE ARCHIMEDES_MODULE_SOURCES CONFIGURE_DEPENDS src2/**.cpp)
file(GLOB_RECURSE ARCHIMEDES_MODULE_MODULES CONFIGURE_DEPENDS src2/**.cppm)
target_sources(archimedes
	PRIVATE "${ARCHIMEDES_MODULE_SOURCES}"
	PUBLIC FILE_SET CXX_MODULES FILES "${ARCHIMEDES_MODULE_MODULES}"
)
target_link_libraries(archimedes PUBLIC
	nvrhi_vk # idk why, but nvrhi_vk needs to be before nvrhi
	${ARCHIMEDES_LIBRARIES}
	GTest::gmock
)
target_include_directories(archimedes PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(archimedes PRIVATE
	$<BUILD_INTERFACE:${ARCHIMEDES_BUILDINFO_DIR}>
)

add_executable(module_test)
target_sources(module_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/module_test/main.cpp)

target_link_libraries(module_test PRIVATE archimedes)

LaunchAddTarget(module_test)
