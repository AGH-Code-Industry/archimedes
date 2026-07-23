#pragma once

#include <concepts>
#include <format>

#include "Exception.h"

template<>
struct std::formatter<arch::Exception> {
private:
	uint32_t _stacktraceDepth = 0;
	bool _hasIdx = false;
	bool _hasStacktrace = false;

	static std::string _stacktraceFmt;
	static std::string parseStacktraceEntry(const std::stacktrace_entry& entry);

public:
	template<class T>
	constexpr uint32_t operator()(const T& val) const;

	constexpr std::format_parse_context::iterator parse(std::format_parse_context& ctx);

	std::format_context::iterator format(const arch::Exception& exception, std::format_context& ctx) const;

	static std::string setStacktraceFormat(const std::string_view fmt) noexcept;
};

template<std::derived_from<arch::Exception> T>
struct std::formatter<T>: public std::formatter<arch::Exception> {};

#include "ExceptionFormatter.hpp"
