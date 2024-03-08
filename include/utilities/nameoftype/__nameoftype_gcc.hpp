#pragma once
#ifdef __GNUC__

#include <source_location>
#include <string>

namespace arch::utils {
template<class T>
constexpr const char* __nameoftype() {
	constexpr auto&& signature_begin = "constexpr std::string_view arch::utils::nameof_type() [with T = ";
	constexpr auto&& signature_end = "; std::string_view = std::basic_string_view<char>]";

	const auto location = std::source_location::current();

	std::string_view name = location.function_name();

	name.remove_prefix(sizeof(signature_begin) - 1);
	name.remove_suffix(sizeof(signature_end) - 1);

	return name;
}
}

#endif