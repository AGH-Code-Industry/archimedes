project "ArchimedesExamples"
    kind "ConsoleApp"
    defines {"ARCHIMEDES_EXAMPLES"}

    include "../build_archimedes"

    staticruntime "On"

    language "C++"
    cppdialect "C++23"

    targetname "ArchimedesExample"
    targetdir "bin/%{cfg.buildcfg}/%{cfg.architecture}"
    
    includedirs {
        -- in-engine includes
        "../include/",
        
        -- GLFW
        "../vendor/glfw/include",

        -- googletest
        "../vendor/gtest/googletest/include",

        -- GLM
        "../vendor/glm",

        -- spdlog
        "../vendor/spdlog/include",

        -- stb
        "../vendor/stb",

        -- ctre
        "../vendor/ctre/include",

        -- openal-soft
        "../vendor/openal-soft/include",

        -- sndfile
        "../vendor/sndfile/include",

        -- freetype
        "../vendor/freetype/include",

        -- jsoncpp
        "../vendor/jsoncpp/include",

        -- volk
        "../vendor/volk",

        -- vulkan-headers
        "../vendor/vulkan-headers/include",

        -- vulkan-memory-allocator
        "../vendor/vulkan-memory-allocator/include",

        -- shaderc
        "../vendor/shaderc/libshaderc/include",
        "../vendor/shaderc/libshaderc_util/include",

        -- nvrhi
        "../vendor/nvrhi/include",

        -- cryptopp
        "../vendor/cryptopp",
        "./"
    }

    files {"./main.cpp", "./**.h"}
    links {"Archimedes"}


    

