workspace "Archimedes"
    configurations {"Debug", "Release"}
    platforms {"Win32", "Win64"}

    include "examples/build_examples.lua"
    include "tests/build_tests.lua"
    include "build_archimedes.lua"

    filter {"platforms:Win32"}
        defines {"WIN_32"}
        system "Windows"
        architecture "x86"

    filter {"platforms:Win64"}
        defines {"WIN_64"}
        system "Windows"
        architecture "x86_64"

    filter {"configurations:Debug"}
        defines {"DEBUG"}
        symbols "Full"

    filter {"configurations:Release"}
        defines {"NDEBUG"}
        optimize "Full"
