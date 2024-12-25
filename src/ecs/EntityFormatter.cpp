#include <ecs/EntityFormatter.h>

std::format_context::iterator std::formatter<arch::ecs::Entity>::format(
	const arch::ecs::Entity entity,
	std::format_context& ctx
) const noexcept {
	const auto width = _widthIdx < 0 ? std::visit_format_arg(*this, ctx.arg(-_widthIdx)) : _width;

	// max length:
	// 0b<48 chars><sep>0b<16 chars>
	static_assert(2 + 48 + 1 + 2 + 16 == 69);
	// I did not made this up
	char entityStr[69]{};
	char* end = nullptr;

	auto id = arch::ecs::_details::EntityTraits::Id::part(entity);

	// <format>'s format strings need to be constexpr
	if (id == arch::ecs::_details::EntityTraits::Id::null) {
		end = std::format_to(entityStr, "null");
	} else if (_prefix) {
		if (_type == 'b') {
			end = std::format_to(entityStr, "{:#b}", id);
		} else if (_type == 'o') {
			end = std::format_to(entityStr, "{:#o}", id);
		} else if (_type == 'x') {
			end = std::format_to(entityStr, "{:#x}", id);
		} else /* if (_type == 'd') */ {
			end = std::format_to(entityStr, "{}", id);
		}
	} else {
		if (_type == 'b') {
			end = std::format_to(entityStr, "{:b}", id);
		} else if (_type == 'o') {
			end = std::format_to(entityStr, "{:o}", id);
		} else if (_type == 'x') {
			end = std::format_to(entityStr, "{:x}", id);
		} else /* if (_type == 'd') */ {
			end = std::format_to(entityStr, "{}", id);
		}
	}

	if (_versionSep) {
		auto ver = arch::ecs::_details::EntityTraits::Version::part(entity);
		if (ver == arch::ecs::_details::EntityTraits::Version::null) {
			end = std::format_to(end, "{}null", _versionSep);
		} else if (_prefix) {
			if (_type == 'b') {
				end = std::format_to(end, "{}{:#b}", _versionSep, ver);
			} else if (_type == 'o') {
				end = std::format_to(end, "{}{:#o}", _versionSep, ver);
			} else if (_type == 'x') {
				end = std::format_to(end, "{}{:#x}", _versionSep, ver);
			} else /* if (_type == 'd') */ {
				end = std::format_to(end, "{}{}", _versionSep, ver);
			}
		} else {
			if (_type == 'b') {
				end = std::format_to(end, "{}{:b}", _versionSep, ver);
			} else if (_type == 'o') {
				end = std::format_to(end, "{}{:o}", _versionSep, ver);
			} else if (_type == 'x') {
				end = std::format_to(end, "{}{:x}", _versionSep, ver);
			} else /* if (_type == 'd') */ {
				end = std::format_to(end, "{}{}", _versionSep, ver);
			}
		}
	}

	const uint32_t len = end - entityStr;

	if (len < width) {
		auto fillLeft = _alignment == '^' ? (width - len) / 2 : (_alignment == '<' ? 0 : width - len);
		auto fillRight = width - len - fillLeft;

		for (; fillLeft; --fillLeft) {
			ctx.out() = _fill;
		}
		for (uint32_t i = 0; i != sizeof(entityStr) && entityStr[i] != '\0'; ++i) {
			ctx.out() = entityStr[i];
		}
		for (; fillRight; --fillRight) {
			ctx.out() = _fill;
		}
	} else {
		for (uint32_t i = 0; i != sizeof(entityStr) && entityStr[i] != '\0'; ++i) {
			ctx.out() = entityStr[i];
		}
	}

	return ctx.out();
}
