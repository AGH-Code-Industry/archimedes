project "ArchimedesUnitTests"
    kind "ConsoleApp"
    defines {"ARCHIMEDES_TESTS"}

    staticruntime "On"

    include "../build_archimedes.lua"

    language "C++"
    cppdialect "C++23"

    targetname "ArchimedesExample"
    targetdir "bin/%{cfg.buildcfg}/%{cfg.architecture}"
    
    files {"tests/**.cpp"}
    includedirs {
        "tests/**.h", 
        "include/**.h", 
        "include/**.hpp"
        -- TODO: Add googletest headers
    }
    
    links {
        "Archimedes"
        -- TODO: Add googletest linking
    }