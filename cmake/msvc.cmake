# MSVC specific behavior

include_guard()

if(MSVC)
    add_compile_options("/permissive-")		# Conform to C++ standard
    add_compile_options("/Zc:preprocessor")	# Conform to C++ standard (preprocessor)
    add_compile_options("/Zc:__cplusplus")	# Set the __cplusplus macro
endif()
