# Macro definiions

include_guard()

target_compile_definitions(archimedes PUBLIC SPDLOG_USE_STD_FORMAT=1)

# Parse & set version
string(SUBSTRING "${ARCHIMEDES_VERSION}" 0 4 ARCHIMEDES_VERSION_YEAR)
string(SUBSTRING "${ARCHIMEDES_VERSION}" 5 2 ARCHIMEDES_VERSION_MONTH)
string(SUBSTRING "${ARCHIMEDES_VERSION}" 8 2 ARCHIMEDES_VERSION_DAY)

target_compile_definitions(archimedes PUBLIC ARCHIMEDES_VERSION_YEAR=${ARCHIMEDES_VERSION_YEAR})
target_compile_definitions(archimedes PUBLIC ARCHIMEDES_VERSION_MONTH=${ARCHIMEDES_VERSION_MONTH})
target_compile_definitions(archimedes PUBLIC ARCHIMEDES_VERSION_DAY=${ARCHIMEDES_VERSION_DAY})
target_compile_definitions(archimedes PUBLIC ARCHIMEDES_VERSION="${ARCHIMEDES_VERSION_YEAR}${ARCHIMEDES_VERSION_MONTH}${ARCHIMEDES_VERSION_DAY}")

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