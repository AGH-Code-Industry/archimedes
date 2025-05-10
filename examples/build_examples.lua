project "ArchimedesExamples"
    kind "WindowedApp"
    defines {"ARCHIMEDES_EXAMPLES"}

    staticruntime "On"

    include "../build_archimedes.lua"

    language "C++"
    cppdialect "C++23"

    targetname "ArchimedesExample"
    targetdir "bin/%{cfg.buildcfg}/%{cfg.architecture}"
    
    files {"examples/**.cpp"}
    includedirs {"examples/**.h", "include/**.h", "include/**.hpp"}
    links {"Archimedes"}


    

