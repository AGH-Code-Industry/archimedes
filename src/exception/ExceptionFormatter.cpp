#include <archimedes/BuildInfo.h>
#include <archimedes/exception/ExceptionFormatter.h>
#include <archimedes/utils/ParseStacktrace.h>

std::string std::formatter<arch::Exception>::_stacktraceFmt =
#if defined(ARCHIMEDES_WINDOWS)
	"  {}\r\n    at {}:{}";
#else
	"  {}\n    at {}:{}";
#endif

std::string std::formatter<arch::Exception>::setStacktraceFormat(const std::string_view fmt) noexcept {
	auto result = std::move(_stacktraceFmt);
	_stacktraceFmt = fmt;
	return result;
}

std::string std::formatter<arch::Exception>::parseStacktraceEntry(const std::stacktrace_entry& entry) {
	auto function = ::arch::utils::parseStacktraceFunction(entry);

	auto line = entry.source_line();
	auto file = entry.source_file();
	if (file.empty()) {
		file = "<unknown>";
	}

	return std::vformat(_stacktraceFmt, std::make_format_args(function, file, line));
}

std::string std::formatter<arch::Exception>::parseSourceLocation(const arch::utils::SimpleSourceLocation& location) {
	auto function = location.functionName();
	auto file = location.fileName();
	auto line = location.line();

	return std::vformat(_stacktraceFmt, std::make_format_args(function, file, line));
}

std::format_context::iterator std::formatter<arch::Exception>::format(
	const arch::Exception& exception,
	std::format_context& ctx
) const {
	auto banner = std::format("[{}]: {}", exception.title(), exception.message());
	for (auto&& c : banner) {
		ctx.out() = c;
	}

	if (_hasStacktrace) {
		auto stacktraceDepth = _hasIdx ? std::visit_format_arg(*this, ctx.arg(_stacktraceDepth)) : _stacktraceDepth;
		if (stacktraceDepth == 0) {
			stacktraceDepth = (uint32_t)-1;
		}

		if (!exception.hasStacktrace()) {
			if constexpr (arch::buildinfo::Compiler::current == arch::buildinfo::Compiler::msvc) {
				ctx.out() = '\r';
			}
			ctx.out() = '\n';

			auto oneLoneEntry = parseSourceLocation(exception.location().get());
			for (auto&& c : oneLoneEntry) {
				ctx.out() = c;
			}
		} else if constexpr (arch::buildinfo::Type::current != arch::buildinfo::Type::Release) {
			auto&& stacktrace = exception.stacktrace().get();
			for (auto&& entry : stacktrace | std::views::take(stacktraceDepth)) {
				if constexpr (arch::buildinfo::Compiler::current == arch::buildinfo::Compiler::msvc) {
					ctx.out() = '\r';
				}
				ctx.out() = '\n';

				auto entryStr = parseStacktraceEntry(entry);
				for (auto&& c : entryStr) {
					ctx.out() = c;
				}
			}
		}
	}

	return ctx.out();
}
