project "Archimedes"
    kind "StaticLib"
    defines {"ARCHIMEDES_LIB"}

    language "C++"
    cppdialect "C++23"

    targetname "Archimedes"
    targetdir "lib/%{cfg.buildcfg}/%{cfg.architecture}"

    files {"src/**.cpp"}
    
    includedirs {
        "include/**.h"
        "include/**.hpp"
        -- TODO: Add vendor include paths
    }

    links {
        -- TODO: Add vendor libraries
    }


