#include <archimedes/utils/ParseStacktrace.h>

namespace arch::utils {

std::string parseStacktraceEntry(const std::stacktrace_entry& entry) {
	std::string function;

	constexpr bool msvc =
#if defined(_MSC_VER)
		true;
#elif defined(__GNUC__)
		false;
#else
		false;
	static_assert("UNSUPPORTED COMPILER");
#endif

	auto desc = entry.description();
	if constexpr (msvc) { // ...!qualified::function<name>+0x...
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
			function = "<unknown>";
		} else {
			function.assign(desc.begin() + begin, desc.begin() + end);
		}
	} else { // qualified::function<name>(...)
		auto end = desc.find('(');
		if (end == desc.npos) {
			end = desc.length();
		}

		if (end == 0) {
			function = "<unknown>";
		} else {
			function.assign(desc.begin(), desc.begin() + end);
		}
	}

	return function;
}

} // namespace arch::utils
