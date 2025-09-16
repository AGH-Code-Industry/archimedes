#pragma once

#include "ThisTCheck.hpp"
#include "ThisTUnique.hpp"

/// @brief Unique ThisT type name macro for internal usage
#define _ARCH_THIS_T ThisT

/// @brief Class attribute creating alias ThisT for T inside class
#define THIS_T(T)                                     \
                                                      \
private:                                              \
	friend struct arch::meta::thisT::UniqueGetter<T>; \
	/* Unique type */                                 \
	struct _ARCH_THIS_T_UNIQUE {};                    \
                                                      \
public:                                               \
	using _ARCH_THIS_T = T;                           \
                                                      \
private:                                              \
	static_assert(arch::meta::thisT::ThisTCheck<T, _ARCH_THIS_T_UNIQUE>, "Used wrong type in THIS_T(T)")

