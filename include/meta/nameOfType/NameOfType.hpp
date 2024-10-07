#pragma once

#include <source_location>

#include "NameOfType.h"

namespace arch::meta::nameOfType {

/// @brief signature<T>() Specialization returning signature with 'int'
template<>
constexpr std::string_view signature<int>() noexcept {
	// Major compilers do not return simply name of function: 'signature'
	// but its signature: 'std::basic_string_view<...> signature<int>() noexcept'
	// this allows to capture position of 'int' in signature
	const auto location = std::source_location::current(
#if __INTELLISENSE__ // Visual Studio cannot into constexpr
		__builtin_LINE(),
		__builtin_COLUMN(),
		__builtin_FILE(),
		__FUNCSIG__
#endif
	);
	return location.function_name();
}

template<class T>
constexpr std::string_view nameOf() noexcept {
	constexpr auto intSignature = signature<int>();
	auto found = intSignature.find("int");
	auto len = intSignature.length();

	auto suffix = intSignature.substr(found + (sizeof("int") - 1));

	// renaming signature -> nameOf
	found -= sizeof("signature");
	found += sizeof("nameOf");

	auto prefix = intSignature.substr(0, found);

	const auto location = std::source_location::current(
#if __INTELLISENSE__ // Visual Studio cannot into constexpr
		__builtin_LINE(),
		__builtin_COLUMN(),
		__builtin_FILE(),
		__FUNCSIG__
#endif
	);
	std::string_view functionName = location.function_name();

	// trimming function signature
	return std::string_view(
		functionName.data() + prefix.length(),
		functionName.length() - prefix.length() - suffix.length()
	);
}

} // namespace arch::meta::nameOfType

#define _ARCH_NAMEOF_TYPE_NO_FMT_IMPL(...) arch::meta::nameOfType::nameOf<__VA_ARGS__>()
/// @brief Returns nonstandardized name of given type (constexpr)
#define nameOfTypeNoFmt(...) _ARCH_NAMEOF_TYPE_NO_FMT_IMPL(__VA_ARGS__)
/// @brief Returns standardized name of given type (runtime only)
#define nameOfType(...) arch::meta::nameOfType::nameOfFmt(nameOfTypeNoFmt(__VA_ARGS__))
