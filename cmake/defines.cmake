# Macro definiions

include_guard()

add_compile_definitions(SPDLOG_USE_STD_FORMAT=1)

# Add macros
if(ARCHIMEDES_DEBUG)
	target_compile_definitions(archimedes PUBLIC
		ARCHIMEDES_DEBUG=1
		ARCHIMEDES_RELEASE=0
	)
else()
	target_compile_definitions(archimedes PUBLIC
		ARCHIMEDES_DEBUG=0
		ARCHIMEDES_RELEASE=1
	)
endif()