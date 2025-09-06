# Adds examples as targets

# Each subdirectory inside 'examples' constitutes a category
# Each subdirectory inside category directory constitutes an example
# Each example is a target of name '<category>-<example_subdir>'
# All .cpp files inside category directory are added associated example target

include_guard()

if(ARCHIMEDES_BUILD_EXAMPLES)
	set(ARCHIMEDES_EXAMPLES_DIRECTORY "${PROJECT_SOURCE_DIR}/examples")

	# scan for files & directories inside 'examples'
	file(GLOB ARCHIMEDES_EXAMPLES_CATEGORIES RELATIVE ${ARCHIMEDES_EXAMPLES_DIRECTORY} "${ARCHIMEDES_EXAMPLES_DIRECTORY}/*")
	
	foreach(ARCHIMEDES_EXAMPLES_CATEGORY ${ARCHIMEDES_EXAMPLES_CATEGORIES})
		# check if directory
		if(IS_DIRECTORY "${ARCHIMEDES_EXAMPLES_DIRECTORY}/${ARCHIMEDES_EXAMPLES_CATEGORY}")
			set(ARCHIMEDES_EXAMPLES_CATEGORY_PATH "${ARCHIMEDES_EXAMPLES_DIRECTORY}/${ARCHIMEDES_EXAMPLES_CATEGORY}")
	
			# scan for examples inside category directory
			file(GLOB ARCHIMEDES_EXAMPLES RELATIVE ${ARCHIMEDES_EXAMPLES_CATEGORY_PATH} "${ARCHIMEDES_EXAMPLES_CATEGORY_PATH}/*")

			foreach(ARCHIMEDES_EXAMPLE ${ARCHIMEDES_EXAMPLES})
				set(ARCHIMEDES_EXAMPLE_PATH "${ARCHIMEDES_EXAMPLES_CATEGORY_PATH}/${ARCHIMEDES_EXAMPLE}")
				# check if directory
				if(IS_DIRECTORY ${ARCHIMEDES_EXAMPLE_PATH})
					# scan for sources
					file(GLOB_RECURSE ARCHIMEDES_EXAMPLE_SOURCES "${ARCHIMEDES_EXAMPLE_PATH}/**.cpp" "${ARCHIMEDES_EXAMPLE_PATH}/**.c")

					# add example target
					set(ARCHIMEDES_EXAMPLE_TARGET "${ARCHIMEDES_EXAMPLES_CATEGORY}-${ARCHIMEDES_EXAMPLE}")
					add_executable(${ARCHIMEDES_EXAMPLE_TARGET} ${ARCHIMEDES_EXAMPLE_SOURCES})

					# link archimedes
					target_link_libraries(${ARCHIMEDES_EXAMPLE_TARGET} PRIVATE archimedes)

					# reuse precompiled header
					target_precompile_headers(${ARCHIMEDES_EXAMPLE_TARGET} REUSE_FROM archimedes)

					# enable IPO
					set_property(TARGET ${ARCHIMEDES_EXAMPLE_TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
			
					# add launch target
					LaunchAddTarget(${ARCHIMEDES_EXAMPLE_TARGET})
				endif()
			endforeach()
		endif()
	endforeach()
endif()