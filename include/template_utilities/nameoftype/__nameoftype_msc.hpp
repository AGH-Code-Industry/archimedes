#pragma once
#ifdef _MSC_VER

#include <source_location>

namespace arch::tmpl_utils {
template<class T>
constexpr std::string_view __nameoftype() {
	constexpr auto&& signature_begin = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl arch::tmpl_utils::__nameoftype<";
	constexpr auto&& signature_end = ">(void)";

	const auto location = std::source_location::current();

#if __INTELLISENSE__ // intellisense's std::source_location::current() is unequal to compiled one, producing IDE errors
	std::string_view name = "class struct " __FUNCSIG__;
#else
	std::string_view name = location.function_name();
#endif

	name.remove_prefix(sizeof(signature_begin) - 1);
	name.remove_suffix(sizeof(signature_end) - 1);

	return name;
}
}

#endif