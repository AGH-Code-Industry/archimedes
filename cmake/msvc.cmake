# MSVC specific behavior

include_guard()

if(MSVC)
    add_compile_options("/permissive-")		# Conform to C++ standard
    add_compile_options("/Zc:preprocessor")	# Conform to C++ standard (preprocessor)
    add_compile_options("/Zc:__cplusplus")	# Set the __cplusplus macro
	
	# Enable more agressive inling in RelWithDebInfo, the /Ob1 setting is legacy yet still the default
	string(REPLACE "/Ob1" "/Ob2" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()
