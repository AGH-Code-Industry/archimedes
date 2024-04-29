#pragma once

#include "RTTIEnabled.hpp"
#include "StaticTypedesc.hpp"
#include <memory>

#include "Typedesc.h"
#include <Logger.h>

namespace arch::meta::rtti {

template<class T>
Helper::operator const T*() const noexcept {
	return (const T*)(nullptr);
}

template<NotDefaultInitializable T>
Helper::operator const T&() const noexcept {
	// this used to avoid null-reference
	return (const T&)*(const T*)(this);
}

template<std::default_initializable T>
Helper::operator T() const noexcept {
	return T();
}

template<class T>
const TypeDescriptor& operator*(const T& lhs, const Helper& rhs) noexcept {
	if constexpr (std::is_polymorphic_v<T> and RTTIEnabled<T>) {
		const TypeDescriptor& td = lhs._getTypeDescriptor();
		if (td != typeid(lhs)) {
			arch::Logger::error(
				"typedesc(): '{}' reference points to object of not RTTIEnabled type",
				static_typedesc(T).name
			);
			arch::Logger::info(
				"hint: check types derived from '{}' for missing EnableRTTI attribute",
				static_typedesc(T).name
			);
		}

		return td;
	} else {
		if constexpr (not RTTIEnabled<T>) {
			arch::Logger::warn("typedesc(): type '{}' is polymorphic, but not RTTIEnabled", static_typedesc(T).name);
		}
		return static_typedesc(T);
	}
}

template<class T>
const TypeDescriptor& ptrToTypeDesc(const T*) noexcept {
	return static_typedesc(T);
}

template<class T>
const T* toPtr(const T& val) noexcept {
	return std::addressof(val);
}

} // namespace arch::meta::rtti

// if ... is a type		-> cast helper to ..., obtain pointer, static_typedesc of pointed type
// if ... is a object	-> multiply ... by helper (obtain TypeDescriptor), convert it to pointer, dereference pointer
#define _typedescImpl(...) \
	arch::meta::rtti::ptrToTypeDesc(arch::meta::rtti::toPtr((__VA_ARGS__) * arch::meta::rtti::helper))

/// @brief Queries TypeDescriptor of given type, may use polymorphism
/// @param ... - type/object which TypeDescriptor to access
#define typedesc(...) (_typedescImpl(__VA_ARGS__))
