include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/conan.cmake")

add_library(${PROJECT_NAME})

include_directories(include)

# find source files
file(GLOB_RECURSE ARCHIMEDES_SOURCE src/**.cpp)
target_sources(${PROJECT_NAME} PUBLIC ${ARCHIMEDES_SOURCE})

# link bindings libs 
file(GLOB_RECURSE Bindings bindings/*.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${Bindings})

# link conan libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${ARCHIMEDES_LIBRARIES})
