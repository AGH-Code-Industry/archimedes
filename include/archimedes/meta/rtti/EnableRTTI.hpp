#pragma once

#include <concepts>

#include <archimedes/meta/ThisT.h>

/// @brief Unique RTTI type name macro for internal usage
#define _ARCH_RTTI_UNIQUE _RTTIUnique

/// @brief Class attribute enabling typedesc() macro usage (requires THIS_T attribute to be set)
/// @brief Obsolete for non-polymorphic types
/// @see std::is_polymorphic_v
/// @details If not used, typedesc() will return incorrect TypeDescriptor when used on base class pointer or reference
#define EnableRTTI                                                                                   \
                                                                                                     \
public:                                                                                              \
	/* Unique type used in RTTIEnabled concept */                                                    \
	template<std::same_as<_ARCH_THIS_T>>                                                             \
	struct _ARCH_RTTI_UNIQUE {                                                                       \
		using Unique = _ARCH_THIS_T_UNIQUE;                                                          \
	};                                                                                               \
                                                                                                     \
private:                                                                                             \
	/* virtual method returning TypeDescriptor */                                                    \
	virtual inline const arch::TypeDescriptor& _getTypeDescriptor() const noexcept {                 \
		static_assert(                                                                               \
			arch::meta::thisT::HasThisT<std::remove_pointer_t<std::remove_cvref_t<decltype(this)>>>, \
			"EnableRTTI requires THIS_T(T) class attribute"                                          \
		);                                                                                           \
		return arch::typedesc<std::remove_pointer_t<std::remove_cvref_t<decltype(this)>>>();         \
	}                                                                                                \
	/* friend operator */                                                                            \
	template<class T>                                                                                \
	requires(arch::meta::rtti::RTTIEnabled<T>)                                                       \
	friend const arch::meta::TypeDescriptor& arch::meta::rtti::typedesc(const T&) noexcept

