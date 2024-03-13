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
    add_definitions(-DARCHIMEDES_WINDOWS=1)
    add_definitions(-DARCHIMEDES_LINUX=0)

    message(STATUS "OS: Windows")
elseif(ARCHIMEDES_LINUX)    
    add_definitions(-DARCHIMEDES_WINDOWS=0)
    add_definitions(-DARCHIMEDES_LINUX=1)

    message(STATUS "OS: Linux")
endif()
