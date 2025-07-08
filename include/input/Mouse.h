#pragma once

#include "MouseButtonCode.h"
#include <Mmath.h>
#include <Ref.h>

#define ARCH_BUTTON(button) static Key button;
#define ARCH_BUTTON_ALIAS(alias, button) static constexpr Key& alias = button;

namespace arch {

class Window;

}

namespace arch::input {

class Key;

class Mouse {
public:

	static constexpr Key& button(const MouseButtonCode code) noexcept;

	ARCH_BUTTON(first)
	ARCH_BUTTON(second)
	ARCH_BUTTON(third)
	ARCH_BUTTON(fourth)
	ARCH_BUTTON(fifth)
	ARCH_BUTTON(sixth)
	ARCH_BUTTON(seventh)
	ARCH_BUTTON(eighth)

	ARCH_BUTTON_ALIAS(left, first)
	ARCH_BUTTON_ALIAS(right, second)
	ARCH_BUTTON_ALIAS(middle, third)

	static inline double x() noexcept;
	static inline double y() noexcept;
	static inline double2 pos() noexcept;

	static inline double dx() noexcept;
	static inline double dy() noexcept;
	static inline double2 dpos() noexcept;

	struct Scroll {
		static inline double x() noexcept;
		static inline double y() noexcept;
	};

	struct Cursor {
		static void disabled(bool value, bool setRaw = true) noexcept;
		static inline bool disabled() noexcept;
		static inline void toggle(bool toggleRaw = true) noexcept;
	};

	struct RawInput {
		static void enabled(bool value) noexcept;
		static inline bool enabled() noexcept;
		static inline void toggle() noexcept;
		static bool supported() noexcept;
	};

private:

	friend class System;

	static void _init(const Ref<Window>& window) noexcept;

	static Ref<Window> _window;
	static double _x;
	static double _y;
	static double _dx;
	static double _dy;
	static double _scrollx;
	static double _scrolly;
	static bool _delta;
	static bool _cursorDisabled;
	static bool _rawInput;
};

} // namespace arch::input

#undef ARCH_BUTTON
#undef ARCH_BUTTON_ALIAS

#include "Mouse.hpp"
