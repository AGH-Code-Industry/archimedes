include_guard()

# msvc specific compile flags
if(MSVC)
    add_compile_options("/permissive-")
    add_compile_options("/Zc:__cplusplus")
    add_compile_options("/Zc:preprocessor")
    add_compile_definitions(SPDLOG_USE_STD_FORMAT=1)
endif()
