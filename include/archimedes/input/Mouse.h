#pragma once

#include "MouseButtonCode.h"
#include <archimedes/Mmath.h>
#include <archimedes/Ref.h>

#define ARCH_BUTTON(button) static Key button;
#define ARCH_BUTTON_ALIAS(alias, button) static constexpr Key& alias = (Key&)button;

namespace arch {

class Window;

}

namespace arch::input {

class Key;
class Scroll;

/// @brief Mouse class
class Mouse {
	class Scroll: public Key {
	public:

		inline Scroll() noexcept = default;

		/// @brief Returns horizontal scroll value
		inline double x() const noexcept;
		/// @brief Returns vertical scroll value
		inline double y() const noexcept;

	private:
		friend class System;

		double _x{};
		double _y{};
	};

	class RawInput;

	class Cursor {
	public:
		/// @brief Sets whether cursor should be disabled: hidden and locked to current window, best for camera control
		/// @param value - whether to disable cursor
		/// @param setRaw (optional) - whether to set raw mouse input, default value true
		void disabled(bool value, bool setRaw = true) noexcept;
		/// @brief Returns whether cursor is disabled
		inline bool disabled() const noexcept;
		/// @brief Toggles whether cursor is disabled
		/// @param toggleRaw (optional) - whether to set raw mouse input when disabling cursor
		inline void toggle(bool toggleRaw = true) noexcept;

	private:
		friend class ::arch::input::Mouse::RawInput;

		bool _disabled;
	};

	class RawInput {
	public:
		/// @brief Sets whether to use raw (unscaled and unaccelerated) mouse motion
		/// @details Available only if cursor is disabled and supportsRaw() returns true
		/// @param value - whether to enable raw mouse motion
		void enabled(bool value) noexcept;
		/// @brief Returns whether raw mouse motion is enabled
		inline bool enabled() const noexcept;
		/// @brief Toggles rawInput
		inline void toggle() noexcept;
		/// @brief Returns whether raw mouse motion is supported
		static bool supported() noexcept;

	private:
		friend class ::arch::input::Mouse::Cursor;

		bool _enabled;
	};

public:
	/// @brief Returns button of given code
	/// @param code - button code to find
	/// @throws std::invalid_argument if code was not found
	static constexpr Key& button(const MouseButtonCode code);

	ARCH_BUTTON(first)
	ARCH_BUTTON(second)
	static Scroll scroll;
	ARCH_BUTTON(fourth)
	ARCH_BUTTON(fifth)
	ARCH_BUTTON(sixth)
	ARCH_BUTTON(seventh)
	ARCH_BUTTON(eighth)

	ARCH_BUTTON_ALIAS(left, first)
	ARCH_BUTTON_ALIAS(right, second)
	ARCH_BUTTON_ALIAS(middle, scroll)
	ARCH_BUTTON_ALIAS(third, scroll)

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

	static Cursor cursor;
	static RawInput rawInput;

private:
	friend class System;

	static void _setWindow(const Ref<Window>& window) noexcept;

	static Ref<Window> _window;
	static double _x;
	static double _y;
	static double _dx;
	static double _dy;
	// whether to compute delta, eg. switching from Cursor::disabled
	static bool _delta;
};

} // namespace arch::input

#undef ARCH_BUTTON
#undef ARCH_BUTTON_ALIAS

#include "Mouse.hpp"
