#pragma once

#include <cassert>
#include <cstdlib>

#include <archimedes/Logger.h>

#if ARCHIMEDES_DEBUG

/// @brief Macro for creating debug-only asserts, logs and calls std::abort() on fail
#define ARCH_ASSERT(cond, msg)                                              \
	if (!(cond)) {                                                          \
		::arch::Logger::error("Assertion `{}` failed: \"{}\"", #cond, msg); \
		::std::abort();                                                     \
	}

#else

/// @brief Macro for creating debug-only asserts, logs and calls std::abort() on fail
#define ARCH_ASSERT(cond, msg) ((void)0)

#endif

/// @brief Macro for creating asserts, logs and calls std::abort() on fail
#define ARCH_FORCE_ASSERT(cond, msg)                                        \
	if (!(cond)) {                                                          \
		::arch::Logger::error("Assertion `{}` failed: \"{}\"", #cond, msg); \
		::std::abort();                                                     \
	}
