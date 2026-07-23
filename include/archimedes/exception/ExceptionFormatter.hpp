#include <concepts>

#include "ExceptionFormatter.h"

template<class T>
constexpr uint32_t std::formatter<arch::Exception>::operator()(const T& val) const {
	if constexpr (std::integral<T>) {
		if constexpr (std::is_signed_v<T>) {
			if (val < 0) {
				throw std::format_error("stacktrace depth is negative");
			}
		}
		return val;
	} else {
		if (!std::is_constant_evaluated()) {
			std::cout << typeid(T).name() << '\n';
		}
		throw std::format_error("stacktrace depth is not an integer");
	}
}

constexpr std::format_parse_context::iterator std::formatter<arch::Exception>::parse(std::format_parse_context& ctx) {
	auto i = ctx.begin();
	auto end = ctx.end();

#define END_CHECK                \
	if (i == end || *i == '}') { \
		return i;                \
	}

	END_CHECK;

	// nested replacement field
	if (*i == '{') {
		auto nestedEnd = std::find(i + 1, end, '}');
		if (nestedEnd == end) {
			throw std::format_error("Stacktrace depth nested replaced field not escaped");
		}

		if (nestedEnd - i == 1) {
			_stacktraceDepth = ctx.next_arg_id();
		} else if (std::from_chars(&*(i + 1), &*nestedEnd, _stacktraceDepth).ec != std::errc()) {
			throw std::format_error("Error parsing stacktrace depth nested replacement field");
		}

		i = nestedEnd + 1;
		_hasStacktrace = true;
	}
	END_CHECK;

	if ('0' <= *i && *i <= '9') {
		_hasStacktrace = true;
		for (;;) {
			END_CHECK;
			if ('0' <= *i && *i <= '9') {
				(_stacktraceDepth *= 10) += *i++ - '0';
			} else {
				break;
			}
		}
	}
	END_CHECK;

	while (i != end && *i != '}') {
		++i;
	}
	return i;
}
