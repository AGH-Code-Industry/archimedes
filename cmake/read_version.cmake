include_guard()

# yyyy.mm.dd: 4(y) + 4(m/d) + 2(.) = 10 chars
file(READ "version.txt" ARCHIMEDES_VERSION LIMIT 10)
string(STRIP ${ARCHIMEDES_VERSION} ARCHIMEDES_VERSION)

if(NOT ARCHIMEDES_VERSION MATCHES "^[0-9][0-9][0-9][0-9]\.[0-9][0-9]\.[0-9][0-9]$")
	message(FATAL_ERROR "'${ARCHIMEDES_VERSION}' is not a valid version")
endif()
