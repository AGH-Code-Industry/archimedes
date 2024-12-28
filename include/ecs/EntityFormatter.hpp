#include "EntityFormatter.h"

constexpr int std::formatter<arch::ecs::Entity>::_svToInt(std::string_view sv) noexcept {
	int result = 0;
	for (auto&& ch : sv) {
		result = result * 10 + (ch - '0');
	}
	return result;
}

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
	auto data = std::string_view(ctx.begin(), ctx.end());

	auto match = ctre::search<
		R"jaPitoleAleBydle(^(?:(?:(.)([\<\>\^])|([\<\>\^])?)(#?)(0?)(?:(?:)|(\d*)|\{(\d*)\}))(?:(?:)|([boxd]?))((.)?v)?(\}))jaPitoleAleBydle">(
		data
	);

	if (!match) {
		throw std::format_error("ecs::Entity formatting sequence error");
	}

	// fill character
	// (.)
	auto fillOpt = match.get<1>().to_optional_view();
	if (fillOpt) {
		_fill = fillOpt.value()[0];

		// alignment
		// ([\<\>\^])
		auto alignOpt = match.get<2>().to_optional_view();
		if (alignOpt) {
			_alignment = alignOpt.value()[0];
		}
	} else {
		// alignment
		// ([\<\>\^])?
		auto alignOpt = match.get<3>().to_optional_view();
		if (alignOpt) {
			_alignment = alignOpt.value()[0];
		} else if (!match.get<5>().to_optional_view().value().empty()) { // fill with zeroes; (0?)
			_fill = '0';
		}
	}

	// prefix indicating base
	// (#?)
	_prefix = match.get<4>().to_view().length();

	// literal width
	// (\d*)
	auto widthOpt = match.get<6>().to_optional_view();
	if (widthOpt) {
		auto value = _svToInt(widthOpt.value());
		if (value < 0) {
			throw std::format_error("width is negative");
		}
		_width = value;
	} else {
		// dynamic width (std::format parameter)
		// \{(\d*)\}
		auto widthIdxOpt = match.get<7>().to_optional_view();
		if (widthIdxOpt) {
			_hasIdx = true;
			if (widthIdxOpt.value().empty()) {
				_widthIdx = -(short)ctx.next_arg_id();
			} else {
				_widthIdx = _svToInt(widthIdxOpt.value());
				if (_widthIdx < 0) {
					throw std::format_error("width is negative");
				}
			}
		}
	}

	// base
	// ([boxd]?)
	auto typeOpt = match.get<8>().to_optional_view();
	if (typeOpt) {
		_type = typeOpt.value()[0];
	}

	// version separator
	// (.)?
	auto versionSepOpt = match.get<10>().to_optional_view();
	if (versionSepOpt && !versionSepOpt.value().empty()) {
		_versionSep = versionSepOpt.value()[0];
	} else {
		// format with version, use default separator '.'
		// ((.)?v)?
		auto versionOpt = match.get<9>().to_optional_view();
		if (versionOpt) {
			_versionSep = '.';
		}
	}

	// format specification ending
	// (\})
	return match.get<11>().begin();
}
