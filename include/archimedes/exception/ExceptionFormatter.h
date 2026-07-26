#pragma once

#include <concepts>
#include <format>

#include "Exception.h"

/// @brief Formatter for arch::Exception and derived classes
template<>
struct std::formatter<arch::Exception> {
public:
	/// @brief Visitor operator, parses stacktrace depth value
	template<class T>
	constexpr uint32_t operator()(const T& val) const;

	/// @brief Parse function
	/// @param ctx - parse context
	constexpr std::format_parse_context::iterator parse(std::format_parse_context& ctx);

	/// @brief Formatting function
	/// @param exception - exception to format
	/// @param ctx - format context
	std::format_context::iterator format(const arch::Exception& exception, std::format_context& ctx) const;

	/// @brief Sets given stacktrace format and returns the old one
	/// @brief Format must contain exactly 3 replacement fields: 1st - function name, 2nd - filename and 3rd - line
	/// @param fmt - format string
	static std::string setStacktraceFormat(const std::string_view fmt) noexcept;

private:
	uint32_t _stacktraceDepth = 0;
	bool _hasIdx = false;
	bool _hasStacktrace = false;

	static std::string _stacktraceFmt;
	// parses function name from stacktrace_entry
	static std::string parseStacktraceEntry(const std::stacktrace_entry& entry);
};

template<std::derived_from<arch::Exception> T>
struct std::formatter<T>: public std::formatter<arch::Exception> {};

#include "ExceptionFormatter.hpp"
