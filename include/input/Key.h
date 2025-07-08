#pragma once

#include "KeyCode.h"
#include "KeyState.h"
#include "MouseButtonCode.h"

namespace arch::input {

class Key {
public:

	using Time = std::chrono::duration<f32>;

	inline Key() noexcept;
	Key(const Key&) = delete;

	static constexpr Key& get(const u32 code);

	/// @brief Returns whether state contains given KeyState
	/// @param keyState - KeyState to check for
	inline bool has(const KeyState keyState) const noexcept;
	/// @brief Returns whether state contains given mask
	/// @param mask - mask to check for
	inline bool has(const u32 mask) const noexcept;

	/// @brief Returns whether key/button is up
	inline bool up() const noexcept;
	/// @brief Returns time the key/button is up
	inline Time upTime() const noexcept;
	/// @brief Returns whether key/button was released
	inline bool released() const noexcept;
	/// @brief Returns whether key/button is down
	inline bool down() const noexcept;
	/// @brief Returns time the key/button is up
	inline Time downTime() const noexcept;
	/// @brief Returns whether key/button was pressed
	inline bool pressed() const noexcept;
	/// @brief Returns whether key/button repeats (is held down)
	inline bool repeat() const noexcept;
	/// @brief Returns whether key/button state changed (pressed or released)
	inline bool changed() const noexcept;

	/// @brief Returns whether key/button state had shift down
	inline bool shift() const noexcept;
	/// @brief Returns whether key/button state had ctrl down
	inline bool ctrl() const noexcept;
	/// @brief Returns whether key/button state had ctrl down
	inline bool control() const noexcept;
	/// @brief Returns whether key/button state had alt down
	inline bool alt() const noexcept;

	/// @brief Returns whether key/button state had super key down
	inline bool super() const noexcept;
	/// @brief Returns whether key/button state had windows key down
	inline bool win() const noexcept;
	/// @brief Returns whether key/button state had windows key down
	inline bool windows() const noexcept;
	/// @brief Returns whether key/button state had command key down
	inline bool command() const noexcept;
	/// @brief Returns whether key/button state had meta key down
	inline bool meta() const noexcept;

	/// @brief Returns whether key/button state had caps lock on
	inline bool capsLock() const noexcept;
	/// @brief Returns whether key/button state had num lock on
	inline bool numLock() const noexcept;

private:

	friend class System;

	u32 _state;
	Time _time;
};

} // namespace arch::input

#include "Key.hpp"
