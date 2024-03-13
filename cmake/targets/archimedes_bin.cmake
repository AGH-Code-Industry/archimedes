include_guard()

include("${PROJECT_SOURCE_DIR}/cmake/library.cmake")

add_executable(${PROJECT_NAME}_bin archimedes_bin/main.cpp)
target_link_libraries(${PROJECT_NAME}_bin ${PROJECT_NAME})
