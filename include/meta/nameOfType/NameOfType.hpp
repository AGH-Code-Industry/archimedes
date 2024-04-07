#pragma once

#include <source_location>

#include "NameOfType.h"

namespace arch {
namespace meta {
namespace _nameOfType {

/// @brief signature<T>() Specialization returning signature with 'int'
template<>
constexpr std::string_view signature<int>() noexcept {
	// Major compilers do not return simply name of function: 'signature'
	// but its signature: 'std::basic_string_view<...> signature<int>() noexcept'
	// this allows to capture position of 'int' in signature
	const auto location = std::source_location::current();
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

	const auto location = std::source_location::current();
	std::string_view functionName = location.function_name();

	// trimming function signature
	return std::string_view(
		functionName.data() + prefix.length(),
		functionName.length() - prefix.length() - suffix.length()
	);
}

} // namespace _nameOfType

using _nameOfType::nameOf;
} // namespace meta

using meta::nameOf;
} // namespace arch

#define _nameOfTypeImpl(x) arch::meta::_nameOfType::nameOf<x>()
/// @brief Returns nonstandardized name of given type
#define nameOfTypeNoFmt(x) _nameOfTypeImpl(x)
/// @brief Returns standardized name of given type
#define nameOfType(x) arch::meta::_nameOfType::nameOfTypeFmt(nameOfTypeNoFmt(x))
