#include <archimedes/BuildInfo.h>
#include <archimedes/utils/ParseStacktrace.h>

namespace arch::utils {

namespace {

std::string parseStacktraceFunctionMSVC(const std::stacktrace_entry& entry) {
	std::string result;

	auto desc = entry.description();

	auto begin = desc.find('!');
	if (begin == desc.npos) {
		begin = 0;
	} else {
		++begin;
	}

	auto end = desc.find('+');
	if (end == desc.npos) {
		end = desc.length();
	}

	if (end == begin) {
		result = "<unknown>";
	} else {
		result.assign(desc.begin() + begin, desc.begin() + end);
	}

	return result;
}

std::string parseStacktraceFunctionGCC(const std::stacktrace_entry& entry) {
	std::string result;

	auto desc = entry.description();

	auto end = desc.find('(');
	if (end == desc.npos) {
		end = desc.length();
	}

	if (end == 0) {
		result = "<unknown>";
	} else {
		result.assign(desc.begin(), desc.begin() + end);
	}

	return result;
}

} // namespace

std::string parseStacktraceFunction(const std::stacktrace_entry& entry) {
	if constexpr (buildinfo::Compiler::current == buildinfo::Compiler::msvc) {
		return parseStacktraceFunctionMSVC(entry);
	} else {
		return parseStacktraceFunctionGCC(entry);
	}
}

} // namespace arch::utils
