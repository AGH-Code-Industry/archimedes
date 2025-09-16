# Extra targets handling

include_guard()

# Include all .cmake files in cmake/targets
file(GLOB_RECURSE ARCHIMEDES_TARGETS_CMAKE cmake/targets/*.cmake)
foreach (ARCHIMEDES_TARGET_CMAKE ${ARCHIMEDES_TARGETS_CMAKE})
    include(${ARCHIMEDES_TARGET_CMAKE})
endforeach ()

LaunchMake()