#include <ecs/EntityFormatter.h>
#include <ecs/EntityOperators.h>

std::format_context::iterator std::formatter<arch::ecs::Entity>::format(
	const arch::ecs::Entity entity,
	std::format_context& ctx
) const noexcept {
	const auto width = (_flags & Flags::hasIdx) ? std::visit_format_arg(*this, ctx.arg(_width)) : _width;

	// max length:
	// 0b<48 bits><1 separator>0b<16 bits>
	static_assert(2 + 48 + 1 + 2 + 16 == 69);
	// I did not made this up
	char entityStr[69]{};
	char* end = nullptr;

#define IF_TYPE(to, type, prefix, ...)                           \
	if (_type == (#type)[0]) {                                   \
		end = std::format_to(to, prefix #type "}", __VA_ARGS__); \
	}

#define ELSE_IF_TYPE(to, type, prefix, ...) else IF_TYPE(to, type, prefix, __VA_ARGS__)

#define FORMAT_WITH_PREFIX(to, prefix, ...)  \
	IF_TYPE(to, b, prefix, __VA_ARGS__)      \
	ELSE_IF_TYPE(to, B, prefix, __VA_ARGS__) \
	ELSE_IF_TYPE(to, o, prefix, __VA_ARGS__) \
	ELSE_IF_TYPE(to, x, prefix, __VA_ARGS__) \
	ELSE_IF_TYPE(to, X, prefix, __VA_ARGS__) \
	ELSE_IF_TYPE(to, d, prefix, __VA_ARGS__)

	auto id = *entity;

	if (id == arch::ecs::nullID) {
		end = std::format_to(entityStr, "null");
	} else if (_flags & Flags::hasPrefix) {
		FORMAT_WITH_PREFIX(entityStr, "{:#", id)
	} else {
		FORMAT_WITH_PREFIX(entityStr, "{:", id)
	}

	if (_versionSep) {
		auto ver = ~entity;
		if (ver == arch::ecs::nullVersion) {
			end = std::format_to(end, "{}null", _versionSep);
		} else if (_flags & Flags::hasPrefix) {
			FORMAT_WITH_PREFIX(end, "{}{:#", _versionSep, ver)
		} else {
			FORMAT_WITH_PREFIX(end, "{}{:", _versionSep, ver)
		}
	}

	const uint32_t len = end - entityStr;

	if (len < width) {
		auto fillLeft = _flags & Flags::alignCenter ? (width - len) / 2 : (_flags & Flags::alignLeft ? 0 : width - len);
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
