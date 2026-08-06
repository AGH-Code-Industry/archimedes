#pragma once

#if defined(_MSC_VER)
#define ARCHIMEDES_FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define ARCHIMEDES_FORCE_INLINE __attribute__((always_inline)) inline
#else
#define ARCHIMEDES_FORCE_INLINE inline
#endif
