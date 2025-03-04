include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/library.cmake")

file(GLOB_RECURSE ARCHIMEDES_BIN_SOURCE CONFIGURE_DEPENDS archimedes_bin/**.cpp)
add_executable(${PROJECT_NAME}_bin ${ARCHIMEDES_BIN_SOURCE})

target_include_directories(${PROJECT_NAME}_bin PUBLIC archimedes_bin)
target_link_libraries(${PROJECT_NAME}_bin PUBLIC ${PROJECT_NAME})