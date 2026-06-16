# CMake options

include_guard()

option(ARCHIMEDES_SHARED
	"Build archimedes as shared library (currently unsupported)"
	OFF
)
if(ARCHIMEDES_SHARED)
	message(FATAL_ERROR
		"archimedes does not currently support building as shared library"
	)
endif()

option(ARCHIMEDES_BUILD_TESTS
	"Build tests"
	ON
)
option(ARCHIMEDES_BUILD_EXAMPLES
	"Build examples"
	ON
)

option(ARCHIMEDES_INSTALL
	"Install archimedes"
	ON
)