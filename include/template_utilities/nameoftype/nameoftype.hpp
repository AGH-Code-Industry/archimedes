#pragma once

#ifdef _MSC_VER // Microsoft compiler
#include "__nameoftype_msc.hpp"
#elif defined __clang__ // clang compiler
#include "__nameoftype_clang.hpp"
#elif defined __GNUC__ // GCC compiler
#include "__nameoftype_gcc.hpp"
#else
static_assert(false, "compiler does not support std::source_location::function_name() with signatures");
#endif

#include <source_location>
#include <string>

namespace arch::tmpl_utils {
template<class T = int>
constexpr std::string_view __signature() = delete;

/// @brief Helper function, provides function signature with int specialization.
///
template<>
constexpr std::string_view __signature<int>() {
	const auto location = std::source_location::current();
	return location.function_name();
}

/// @brief Returns name of type T.
/// @tparam T - type to get type of.
template<class T>
constexpr std::string_view nameoftype() {
	auto int_signature = __signature();
	auto found = int_signature.find("int");
	auto len = int_signature.length();

	auto suffix = int_signature.substr(found + 3);
	found -= sizeof("__signature");
	found += sizeof("nameoftype");
	auto prefix = int_signature.substr(0, found);

	const auto location = std::source_location::current();
	std::string_view fn_name = location.function_name();

	return std::string_view(fn_name.data() + prefix.length(), fn_name.length() - prefix.length() - suffix.length());
}

/// @brief Hepler value of nameoftype<T>().
///
template<class T>
static constexpr std::string_view nameoftype_v = nameoftype<T>();
}