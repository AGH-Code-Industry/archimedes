#include <archimedes/utils/ParseStacktrace.h>

namespace arch::utils {

std::string parseStacktraceFunction(const std::stacktrace_entry& entry) {
	std::string result;

	constexpr bool msvc =
#if defined(_MSC_VER)
		true;
#elif defined(__GNUC__)
		false;
#else
		false;
	static_assert(false, "UNSUPPORTED COMPILER");
#endif

	// function name surrounded with []
	auto desc = entry.description();
	if constexpr (msvc) { // ...![qualified::function<name>]+0x...
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
	} else /* if(gcc) */ { // [qualified::function<name>](...)
		auto end = desc.find('(');
		if (end == desc.npos) {
			end = desc.length();
		}

		if (end == 0) {
			result = "<unknown>";
		} else {
			result.assign(desc.begin(), desc.begin() + end);
		}
	}

	return result;
}

} // namespace arch::utils
