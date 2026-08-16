#pragma once

#include <format>
#include <source_location>

#include <archimedes/utils/SimpleSourceLocation.h>

namespace arch::utils {

/// @brief Replacement for std::basic_format_string, allowing for automatic capture of source_location
/// @tparam Char - character type
/// @tparam Args... - arguments
template<class SourceLocation, class Char, class... Args>
struct BasicSourceAwareFormatString {
	/// @brief Format string to be passed to std::format
	const std::basic_format_string<Char, Args...> fmt;
	/// @brief Source location to be passed
	const SourceLocation loc;

	/// @brief Constructor
	/// @details You will never call this manually, format string will get converted with it
	/// @tparam T - format string type
	/// @param str - format string
	/// @param location - source_location to be captured, do not change
	template<class T>
	requires std::convertible_to<const T&, std::basic_string_view<Char>>
	consteval BasicSourceAwareFormatString(const T& str, const SourceLocation location = SourceLocation::current());
};

/// @brief Specialization to char type
/// @details type_identity inhibits template argument deduction via format string, instead deducing them via args...
/// @tparam Args... - argument types
template<class... Args>
using SourceAwareFormatString =
	BasicSourceAwareFormatString<utils::SimpleSourceLocation, char, std::type_identity_t<Args>...>;

/// @brief Specialization to wchar_t (wide char) type
/// @details type_identity inhibits template argument deduction via format string, instead deducing them via args...
/// @tparam Args... - argument types
template<class SourceLocation, class... Args>
using SourceAwareWFormatString =
	BasicSourceAwareFormatString<utils::SimpleSourceLocation, wchar_t, std::type_identity_t<Args>...>;

} // namespace arch::utils

#include "SourceAwareFormatString.hpp"
