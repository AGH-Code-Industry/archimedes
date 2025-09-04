# Macro definiions

include_guard()

add_compile_definitions(SPDLOG_USE_STD_FORMAT=1)
# Parse & set version
string(REPLACE "." "" ARCHIMEDES_VERSION_INT "${ARCHIMEDES_VERSION}")
target_compile_definitions(archimedes PUBLIC ARCHIMEDES_VERSION=${ARCHIMEDES_VERSION_INT})

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

# Add macros
if(ARCHIMEDES_WINDOWS)    
    target_compile_definitions(archimedes PUBLIC
		ARCHIMEDES_WINDOWS=1
		ARCHIMEDES_LINUX=0
		ARCHIMEDES_UNIX=0
	)
elseif(ARCHIMEDES_LINUX)    
   target_compile_definitions(archimedes PUBLIC
		ARCHIMEDES_WINDOWS=0
		ARCHIMEDES_LINUX=1
		ARCHIMEDES_UNIX=1
	)
endif()