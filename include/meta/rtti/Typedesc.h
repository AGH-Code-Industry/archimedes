#pragma once

#include <concepts>
#include <type_traits>

#include "TypeDescriptor.h"

namespace arch::meta::rtti::_details {

/// @brief Helper struct of typedesc() macro
struct Helper {
	/// @brief Pointer casting operator, used only for type deduction
	/// @tparam T - type to which pointer cast to
	/// @return nullptr
	template<class T>
	operator const T*() const noexcept;
	/// @brief Reference casting operator, used only for type deduction
	/// @tparam T - type to which refernce cast to
	/// @return Casted (*this)
	template<class T>
	requires(not std::default_initializable<T>)
	operator const T&() const noexcept;
	// #ifndef _MSC_VER // works without it on MSVC
	/// @brief Casting operator, used only for type deduction
	/// @tparam T - type to which refernce cast to
	/// @return T()
	template<std::default_initializable T>
	operator T() const noexcept;
	// #endif
	/// @brief Helper dereference operator
	/// @return (*this)
	Helper operator*() const noexcept;
};

/// @brief Instance of Helper object
static inline constexpr Helper helper{};

/// @brief Helper multiplication operator
/// @brief If T is polymorphic and RTTIEnabled, obtains TypeDescriptor from lhs
/// @brief Else obtains TypeDescriptor by staticTypedesc(T)
/// @tparam T - type of lhs
/// @param lhs - object which TypeDescriptor to obtain
/// @param rhs - Helper object
template<class T>
const TypeDescriptor& operator*(const T& lhs, const Helper& rhs) noexcept;

/// @brief Obtains TypeDescriptor of type pointed to by argument
/// @tparam T - type whose TypeDescriptor needs to be obtained
/// @param <unnamed> - pointer to object whose TypeDescriptor needs to be obtained.
/// @return Obtained TypeDescriptor
template<class T>
const TypeDescriptor& ptrToTypeDesc(const T*) noexcept;

/// @brief Specialization of ptrToTypeDesc for TypeDescriptor, dereferences given TypeDescriptor
/// @param desc - TypeDescriptor to dereference
template<>
const TypeDescriptor& ptrToTypeDesc<TypeDescriptor>(const TypeDescriptor* desc) noexcept;

/// @brief Const version of std::addressof
/// @tparam T - type of object to obtain address of
/// @param val - object to obtain address of
/// @return Pointer to val
template<class T>
const T* toPtr(const T& val) noexcept;

} // namespace arch::meta::rtti::_details

#include "Typedesc.hpp"
