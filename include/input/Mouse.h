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

/// @brief Mouse class
class Mouse {
public:

	/// @brief Returns button of given code
	/// @param code - button code to find
	/// @throws std::invalid_argument if code was not found
	static constexpr Key& button(const MouseButtonCode code);

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

	/// @brief Returns cursor position on X axis
	static inline double x() noexcept;
	/// @brief Returns cursor position on Y axis
	static inline double y() noexcept;
	/// @brief Returns cursor position
	static inline double2 pos() noexcept;

	/// @brief Returns change of cursor position on X axis
	static inline double dx() noexcept;
	/// @brief Returns change of cursor position on Y axis
	static inline double dy() noexcept;
	/// @brief Returns change of cursor position
	static inline double2 dpos() noexcept;

	struct Scroll {
		/// @brief Returns horizontal scroll value
		static inline double x() noexcept;
		/// @brief Returns vertical scroll value
		static inline double y() noexcept;
	};

	struct Cursor {
		/// @brief Sets whether cursor should be disabled: hidden and locked to current window, best for camera control
		/// @param value - whether to disable cursor
		/// @param setRaw (optional) - whether to set raw mouse input, default value true
		static void disabled(bool value, bool setRaw = true) noexcept;
		/// @brief Returns whether cursor is disabled
		static inline bool disabled() noexcept;
		/// @brief Toggles whether cursor is disabled
		/// @param toggleRaw (optional) - whether to set raw mouse input when disabling cursor
		static inline void toggle(bool toggleRaw = true) noexcept;
	};

	struct RawInput {
		/// @brief Sets whether to use raw (unscaled and unaccelerated) mouse motion
		/// @details Available only if cursor is disabled and supportsRaw() returns true
		/// @param value - whether to enable raw mouse motion
		static void enabled(bool value) noexcept;
		/// @brief Returns whether raw mouse motion is enabled
		static inline bool enabled() noexcept;
		/// @brief Toggles rawInput
		static inline void toggle() noexcept;
		/// @brief Returns whether raw mouse motion is supported
		static bool supported() noexcept;
	};

private:

	friend class System;

	static void _setWindow(const Ref<Window>& window) noexcept;

	static Ref<Window> _window;
	static double _x;
	static double _y;
	static double _dx;
	static double _dy;
	static double _scrollx;
	static double _scrolly;
	// whether to compute delta, eg. switching from Cursor::disabled
	static bool _delta;
	static bool _cursorDisabled;
	static bool _rawInput;
};

} // namespace arch::input

#undef ARCH_BUTTON
#undef ARCH_BUTTON_ALIAS

#include "Mouse.hpp"
