# Adds archimedes_tests target with tests

include_guard()

if(ARCHIMEDES_BUILD_TESTS)
	add_executable(archimedes_tests)
	
	# find & add test sources (all .cpp files in tests directory)
	file(GLOB_RECURSE ARCHIMEDES_TEST_SOURCE tests/**.cpp)
	target_sources(archimedes_tests PRIVATE ${ARCHIMEDES_TEST_SOURCE})

	# link archimedes
	target_link_libraries(archimedes_tests PRIVATE ${PROJECT_NAME})

	# reuse precompiled header
	target_precompile_headers(archimedes_tests REUSE_FROM ${PROJECT_NAME})

	#enable IPO
	set_property(TARGET archimedes_tests PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

	# add launch target
	LaunchAddTarget(archimedes_tests)
endif()