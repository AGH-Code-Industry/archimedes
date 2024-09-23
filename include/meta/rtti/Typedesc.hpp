#pragma once

#include "RTTIEnabled.hpp"
#include "StaticTypedesc.hpp"
#include <memory>

#include "Typedesc.h"
#include <Logger.h>

namespace arch::meta::rtti::_details {

template<class T>
Helper::operator const T*() const noexcept {
	return (const T*)(nullptr);
}

template<class T>
requires(!std::default_initializable<T> && std::copy_constructible<T> && !std::move_constructible<T>)
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
	if constexpr (std::is_polymorphic_v<T> && RTTIEnabled<T>) {
		const TypeDescriptor& td = lhs._getTypeDescriptor();
		if (td != typeid(lhs)) {
			arch::Logger::error(
				"typedesc(): '{}' reference points to object of not RTTIEnabled type",
				staticTypedesc(T).name
			);
			arch::Logger::info(
				"hint: check types derived from '{}' for missing EnableRTTI attribute",
				staticTypedesc(T).name
			);
		}

		return td;
	} else {
		if constexpr (!RTTIEnabled<T>) {
			arch::Logger::warn("typedesc(): type '{}' is polymorphic, but not RTTIEnabled", staticTypedesc(T).name);
		}
		return staticTypedesc(T);
	}
}

template<class T>
const TypeDescriptor& ptrToTypeDesc(const T*) noexcept {
	return staticTypedesc(T);
}

template<class T>
const T* toPtr(const T& val) noexcept {
	return std::addressof(val);
}

} // namespace arch::meta::rtti::_details

// if ... is a type		-> cast helper to ..., obtain pointer, staticTypedesc of pointed type
// if ... is a object	-> multiply ... by helper (obtain TypeDescriptor), convert it to pointer, dereference pointer
#define _ARCH_TYPEDESC_IMPL(...)                                                              \
	arch::meta::rtti::_details::ptrToTypeDesc(                                                \
		arch::meta::rtti::_details::toPtr((__VA_ARGS__) * arch::meta::rtti::_details::helper) \
	)

/// @brief Queries TypeDescriptor of given type, may use polymorphism
/// @param ... - type/object which TypeDescriptor to access
#define typedesc(...) (_ARCH_TYPEDESC_IMPL(__VA_ARGS__))
