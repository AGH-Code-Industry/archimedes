# OS handling file

include_guard()

# Check for OS
if(WIN32)
    set(ARCHIMEDES_WINDOWS TRUE)
    set(ARCHIMEDES_LINUX FALSE)

    message(STATUS "OS: Windows")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(ARCHIMEDES_WINDOWS FALSE)
    set(ARCHIMEDES_LINUX TRUE)

    message(STATUS "OS: Linux")
endif()
