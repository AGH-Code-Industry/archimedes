project "Archimedes"

    local function msvc_crt()
        return '%{cfg.buildcfg == "Debug" and "MultiThreadedDebug" or "MultiThreaded"}'
    end

    -- project type
    kind "StaticLib"

    -- defined constants
    defines {"ARCHIMEDES_WINDOWS=1"}

    staticruntime "On"

    -- language setup
    language "C++"
    cppdialect "C++23"

    -- target settings
    targetname "Archimedes"
    targetdir "lib/%{cfg.buildcfg}/%{cfg.architecture}"

    -- source files
    files {"src/**.cpp"}

    prebuildcommands {
        -- GLFW 
        "cmake -S ./vendor/glfw -B ./vendor/glfw/build -DCMAKE_GENERATOR_PLATFORM=x64 -DGLFW_BUILD_DOCS=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY=" .. msvc_crt(),
        "cmake --build ./vendor/glfw/build --config %{cfg.buildcfg}",

        -- googletest
        "cmake -S ./vendor/gtest -B ./vendor/gtest/build -DCMAKE_GENERATOR_PLATFORM=x64 -Dgtest_force_shared_crt=OFF",
        "cmake --build ./vendor/gtest/build --config %{cfg.buildcfg}",

        -- spdlog
        "cmake -S ./vendor/spdlog -B ./vendor/spdlog/build -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_MSVC_RUNTIME_LIBRARY=" .. msvc_crt(),
        "cmake --build ./vendor/spdlog/build --config %{cfg.buildcfg}",

        -- openal-soft
        "cmake -S ./vendor/openal-soft -B ./vendor/openal-soft/build -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_MSVC_RUNTIME_LIBRARY=" .. msvc_crt(),
        "cmake --build ./vendor/openal-soft/build --config %{cfg.buildcfg}",

        -- sndfile
        "cmake -S ./vendor/sndfile -B ./vendor/sndfile/build -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_MSVC_RUNTIME_LIBRARY=" .. msvc_crt(),
        "cmake --build ./vendor/sndfile/build --config %{cfg.buildcfg}",

        -- freetype
        "cmake -S ./vendor/freetype -B ./vendor/freetype/build -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_MSVC_RUNTIME_LIBRARY=" .. msvc_crt(),
        "cmake --build ./vendor/freetype/build --config %{cfg.buildcfg}",

        -- jsoncpp
        "cmake -S ./vendor/jsoncpp -B /vendor/jsoncpp/build -DCMAKE_GENERATOR_PLATFORM=x64 -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY=" .. msvc_crt(),
        "cmake --build ./vendor/jsoncpp/build --config %{cfg.buildcfg}",

        -- vulkan-memory-allocator
        "cmake -S ./vendor/vulkan-memory-allocator -B /vendor/vulkan-memory-allocator/build -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_MSVC_RUNTIME_LIBRARY=" .. msvc_crt(),
        "cmake --build ./vendor/vulkan-memory-allocator/build --config %{cfg.buildcfg}",

        -- shaderc
        "python vendor/shaderc/utils/git-sync-deps",
        "cmake -S ./vendor/shaderc -B ./vendor/shaderc/build -DSHADERC_SKIP_TESTS=ON -DSHADERC_SKIP_INSTALL=ON -DCMAKE_MSVC_RUNTIME_LIBRARY=" .. msvc_crt(),
        "cmake --build ./vendor/shaderc/build --config %{cfg.buildcfg}",

        -- nvrhi
        "cmake -S ./vendor/nvrhi -B /vendor/nvrhi/build -DNVRHI_WITH_VULKAN=ON -DCMAKE_MSVC_RUNTIME_LIBRARY=" .. msvc_crt(),
        "cmake --build ./vendor/nvrhi/build --config %{cfg.buildcfg}",

        -- cryptopp
        "msbuild vendor/cryptopp/cryptlib.vcxproj /p:configuration=%{cfg.buildcfg} /p:platform=x64 /p:RuntimeLibrary=" .. msvc_crt()

    }
    
    includedirs {
        -- in-engine includes
        "include/",
        
        -- GLFW
        "vendor/glfw/include",

        -- googletest
        "vendor/gtest/googletest/include",

        -- GLM
        "vendor/glm",

        -- spdlog
        "vendor/spdlog/include",

        -- stb
        "vendor/stb",

        -- ctre
        "vendor/ctre/include",

        -- openal-soft
        "vendor/openal-soft/include",

        -- sndfile
        "vendor/sndfile/include",

        -- freetype
        "vendor/freetype/include",

        -- jsoncpp
        "vendor/jsoncpp/include",

        -- volk
        "vendor/volk",

        -- vulkan-headers
        "vendor/vulkan-headers/include",

        -- vulkan-memory-allocator
        "vendor/vulkan-memory-allocator/include",

        -- shaderc
        "vendor/shaderc/libshaderc/include",
        "vendor/shaderc/libshaderc_util/include",

        -- nvrhi
        "vendor/nvrhi/include",

        -- cryptopp
        "vendor/cryptopp" 
    }

    libdirs {
        -- GLFW
        "vendor/glfw/build/src/%{cfg.buildcfg}",

        -- googletest
        "./vendor/gtest/build/lib/%{cfg.buildcfg}",

        -- spdlog
        "vendor/spdlog/build/%{cfg.buildcfg}",

        -- openal-soft
        "vendor/openal-soft/build/%{cfg.buildcfg}",

        -- sndfile
        "vendor/sndfile/build/%{cfg.buildcfg}",

        -- freetype
        "vendor/freetype/build/%{cfg.buildcfg}",

        -- jsoncpp
        "vendor/jsoncpp/build/lib/%{cfg.buildcfg}",
        
        -- vulkan-memory-allocator
        "vendor/vulkan-memory-allocator/build/src/%{cfg.buildcfg}",

        -- shaderc
        "vendor/shaderc/build/libshaderc/%{cfg.buildcfg}",
        "vendor/shaderc/build/libshaderc_util/%{cfg.buildcfg}",

        -- nvrhi
        "vendor/nvrhi/build/%{cfg.buildcfg}",

        -- cryptopp
        "vendor/cryptopp/x64/Output/%{cfg.buildcfg}"

    }

    links {
        "glfw3",
        "gtest", "gtest_main",
        "spdlog",
        "OpenAL32", -- maybe common.lib?
        "sndfile",
        "freetype",
        "jsoncpp",
        "VulkanMemoryAllocator",
        "shaderc", "shaderc_util",
        "nvrhi", "nvrhi_vk",
        "cryptlib"
    }

    characterset("ASCII")

    filter {"platforms:Win64"}
        defines {"ARCHIMEDES_WINDOWS=1"}


