include_guard()

# check for OS
if(WIN32)
    set(ARCHIMEDES_WINDOWS TRUE)
    set(ARCHIMEDES_LINUX FALSE)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(ARCHIMEDES_WINDOWS FALSE)
    set(ARCHIMEDES_LINUX TRUE)
endif()

# set user home directory
if(ARCHIMEDES_WINDOWS)    
    add_compile_definitions(ARCHIMEDES_WINDOWS=1)
    add_compile_definitions(ARCHIMEDES_LINUX=0)
    add_compile_definitions(ARCHIMEDES_UNIX=0)

    message(STATUS "OS: Windows")
elseif(ARCHIMEDES_LINUX)    
    add_compile_definitions(ARCHIMEDES_WINDOWS=0)
    add_compile_definitions(ARCHIMEDES_LINUX=1)
    add_compile_definitions(ARCHIMEDES_UNIX=1)

    message(STATUS "OS: Linux")
endif()
