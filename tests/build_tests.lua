project "ArchimedesUnitTests"

    -- project type
    kind "ConsoleApp"

    -- defined constants
    defines {"ARCHIMEDES_TESTS"}

    staticruntime "On"

    -- language setup
    language "C++"
    cppdialect "C++23"

    -- target settings
    targetname "ArchimedesTests"
    targetdir "bin/%{cfg.buildcfg}/%{cfg.architecture}"
    

    -- soruce files
    files {"../tests/**.cpp"}

    -- include directories
    includedirs {        
        -- engine headers
        "../include",
        
        -- gtest headers
        "../vendor/gtest/googletest/include",
        
        -- GLFW
        "../vendor/glfw/include",

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
        "../vendor/cryptopp"   
    }

    libdirs{
        "../vendor/gtest/build/lib/%{cfg.buildcfg}"
    }


    -- linked libraries
    links {
        "Archimedes",
        "gtest", "gtest_main"
    }

    filter {"platforms:Win64"}
        defines {"ARCHIMEDES_WINDOWS=1"}