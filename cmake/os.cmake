# OS handling file

include_guard()

# Check for OS
if(WIN32)
    set(ARCHIMEDES_WINDOWS TRUE)
    set(ARCHIMEDES_LINUX FALSE)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(ARCHIMEDES_WINDOWS FALSE)
    set(ARCHIMEDES_LINUX TRUE)
endif()

# Add macros
if(ARCHIMEDES_WINDOWS)    
    target_compile_definitions(archimedes PUBLIC
		ARCHIMEDES_WINDOWS=1
		ARCHIMEDES_LINUX=0
		ARCHIMEDES_UNIX=0
	)

    message(STATUS "OS: Windows")
elseif(ARCHIMEDES_LINUX)    
   target_compile_definitions(archimedes PUBLIC
		ARCHIMEDES_WINDOWS=0
		ARCHIMEDES_LINUX=1
		ARCHIMEDES_UNIX=1
	)

    message(STATUS "OS: Linux")
endif()
