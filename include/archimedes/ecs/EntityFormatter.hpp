#include "EntityFormatter.h"

template<class T>
constexpr uint32_t std::formatter<arch::ecs::Entity>::operator()(const T val) const {
	if constexpr (std::integral<T>) {
		if constexpr (std::is_signed_v<T>) {
			if (val < 0) {
				throw std::format_error("width index is negative");
			}
		}
		return val;
	} else {
		if (!std::is_constant_evaluated()) {
			std::cout << typeid(T).name() << '\n';
		}
		throw std::format_error("width index is not an integer");
	}
}

constexpr std::format_parse_context::iterator std::formatter<arch::ecs::Entity>::parse(std::format_parse_context& ctx) {
	// https://en.cppreference.com/w/cpp/utility/format/spec.html

	auto i = ctx.begin();
	auto end = ctx.end();

#define END_CHECK                \
	if (i == end || *i == '}') { \
		return i;                \
	}

	END_CHECK

	// fill & align
	unsigned char alignFlags = 0;

	auto alignIt = i + 1;
	if (alignIt == end) {
		alignIt == i;
	}

	for (;;) {
		switch (*alignIt) {
			case '<': alignFlags |= Flags::alignLeft; break;
			case '^': alignFlags |= Flags::alignCenter; break;
			case '>': alignFlags |= Flags::alignRight; break;
		}

		if (alignFlags) {
			if (alignIt != i) { // has fill
				if (*i == '{' || *i == '}') {
					throw std::format_error("'{' and '}' are not allowed for fill");
				}
				_fill = *i;
				i = alignIt + 1;
			} else {
				++i;
			}
			_flags |= alignFlags;
			break;
		} else if (alignIt == i) {
			break;
		}
		alignIt = i;
	}
	END_CHECK

	// base prefix
	if (*i == '#') {
		_flags |= Flags::hasPrefix;
		++i;
	}
	END_CHECK

	// fill with zeros
	// align overrides zeros
	if (*i == '0' && !(_flags & Flags::align)) {
		_fill = '0';
		++i;
	}
	END_CHECK

	// width nested replacement field
	if (*i == '{') {
		auto nestedEnd = std::find(i + 1, end, '}');
		if (nestedEnd == end) {
			throw std::format_error("Width nested replacement field not escaped");
		}

		if (nestedEnd - i == 1) {
			_width = ctx.next_arg_id();
		} else if (std::from_chars(&*(i + 1), &*nestedEnd, _width).ec != std::errc()) {
			throw std::format_error("Error parsing width nested replacement field");
		}

		_flags |= Flags::hasIdx;
		i = nestedEnd + 1;
	}

	// width literal
	for (;;) {
		END_CHECK
		if ('0' <= *i && *i <= '9') {
			(_width *= 10) += *i++ - '0';
		} else {
			break;
		}
	}
	END_CHECK

	// type
	switch (*i) {
		case 'b':
		case 'B':
		case 'o':
		case 'x':
		case 'X':
		case 'd':
			_type = *i;
			++i;
			break;
		default: _type = 'd'; break;
	}
	END_CHECK

	// version
	if (*i == 'v') {
		_versionSep = '.';
		++i;

		END_CHECK else {
			if (*i == '{') {
				throw std::format_error("'{' is not allowed as version separator");
			}
			_versionSep = *i;
		}
		++i;
	}
	END_CHECK

	// set default alignment
	if (!(_flags & Flags::align)) {
		_flags |= Flags::alignRight;
	}

	while (i != end && *i != '}') {
		++i;
	}
	return i;

	//	R"jaPitoleAleBydle(^(?:(?:(.)([\<\>\^])|([\<\>\^])?)(#?)(0?)(?:(?:)|(\d*)|\{(\d*)\}))(?:(?:)|([boxd]?))((.)?v)?(\}))jaPitoleAleBydle"
}
