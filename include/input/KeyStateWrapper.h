#pragma once

#include "KeyState.h"

namespace arch::input {

/// @brief Wrapper around KeyState bitmask and time
struct KeyStateWrapper {
	/// @brief Type of time
	using Time = std::chrono::duration<f32>;

	/// @brief Bitmask of state
	const u32 value = *KeyState::up;
	/// @brief Time of state
	const Time time{};

	/// @brief Returns whether state contains given KeyState
	/// @param keyState - KeyState to check for
	constexpr bool has(const KeyState::Value keyState) const noexcept;
	/// @brief Returns whether state contains given mask
	/// @param mask - mask to check for
	constexpr bool has(const u32 mask) const noexcept;

	/// @brief Returns whether key/button is up
	constexpr bool up() const noexcept;
	/// @brief Returns time the key/button is up
	constexpr Time upTime() const noexcept;
	/// @brief Returns whether key/button was released
	constexpr bool released() const noexcept;
	/// @brief Returns whether key/button is down
	constexpr bool down() const noexcept;
	/// @brief Returns time the key/button is up
	constexpr Time downTime() const noexcept;
	/// @brief Returns whether key/button was pressed
	constexpr bool pressed() const noexcept;
	/// @brief Returns whether key/button repeats (is held down)
	constexpr bool repeat() const noexcept;
	/// @brief Returns whether key/button state changed (pressed or released)
	constexpr bool changed() const noexcept;

	/// @brief Returns whether key/button state had shift down
	constexpr bool shift() const noexcept;
	/// @brief Returns whether key/button state had ctrl down
	constexpr bool ctrl() const noexcept;
	/// @brief Returns whether key/button state had ctrl down
	constexpr bool control() const noexcept;
	/// @brief Returns whether key/button state had alt down
	constexpr bool alt() const noexcept;

	/// @brief Returns whether key/button state had super key down
	constexpr bool super() const noexcept;
	/// @brief Returns whether key/button state had windows key down
	constexpr bool win() const noexcept;
	/// @brief Returns whether key/button state had windows key down
	constexpr bool windows() const noexcept;
	/// @brief Returns whether key/button state had command key down
	constexpr bool command() const noexcept;
	/// @brief Returns whether key/button state had meta key down
	constexpr bool meta() const noexcept;

	/// @brief Returns whether key/button state had caps lock on
	constexpr bool capsLock() const noexcept;
	/// @brief Returns whether key/button state had num lock on
	constexpr bool numLock() const noexcept;
};

} // namespace arch::input

#include "KeyStateWrapper.hpp"
