#pragma once
#ifdef __clang__

#include <source_location>

namespace arch::tmpl_utils {
template<class T>
constexpr const char* __nameoftype() {
	constexpr auto&& signature_begin = "std::string_view arch::tmpl_utils::nameof_type() [";
	constexpr auto&& signature_end = "]";

	const auto location = std::source_location::current();

	std::string_view name = location.function_name();

	name.remove_prefix(sizeof(signature_begin) - 1);
	name.remove_suffix(sizeof(signature_end) - 1);

	return name;
}
}

#endif