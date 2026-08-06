#pragma once

#ifndef SPDLOG_USE_STD_FORMAT

#define SPDLOG_USE_STD_FORMAT 1

#endif

#ifndef SPDLOG_COMPILED_LIB

#define SPDLOG_COMPILED_LIB 1

#endif

#if defined(_MSC_VER) && defined(_WIN32)

#define ARCHIMEDES_WINDOWS 1
#define ARCHIMEDES_LINUX 0
#define ARCHIMEDES_UNIX 0

#elif defined(__GNUC__) && defined(__linux__)

#define ARCHIMEDES_WINDOWS 0
#define ARCHIMEDES_LINUX 1
#define ARCHIMEDES_UNIX 1

#endif

#if defined(NDEBUG)

#define ARCHIMEDES_DEBUG 0
#define ARCHIMEDES_RELEASE 1

#else

#define ARCHIMEDES_DEBUG 1
#define ARCHIMEDES_RELEASE 0

#endif
