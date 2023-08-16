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

namespace arch::tmpl_utils {
/// @brief Returns name of type T.
/// @tparam T - type to get type of.
template<class T>
constexpr std::string_view nameoftype() {
	return __nameoftype<T>();
}

/// @brief Hepler value of nameoftype<T>().
///
template<class T>
static constexpr std::string_view nameoftype_v = nameoftype<T>();
}