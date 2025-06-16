#pragma once

#include "KeyState.h"

namespace arch::input {

struct KeyStateWrapper {
	using Time = std::chrono::duration<f32>;

	const u32 value = *KeyState::up;
	const Time time{};

	constexpr bool up() const noexcept;
	constexpr Time upTime() const noexcept;
	constexpr bool released() const noexcept;
	constexpr bool down() const noexcept;
	constexpr Time downTime() const noexcept;
	constexpr bool pressed() const noexcept;
	constexpr bool repeat() const noexcept;

	constexpr bool shift() const noexcept;
	constexpr bool ctrl() const noexcept;
	constexpr bool control() const noexcept;
	constexpr bool alt() const noexcept;

	constexpr bool super() const noexcept;
	constexpr bool win() const noexcept;
	constexpr bool windows() const noexcept;
	constexpr bool command() const noexcept;
	constexpr bool meta() const noexcept;

	constexpr bool capsLock() const noexcept;
	constexpr bool numLock() const noexcept;
};

} // namespace arch::input

#include "KeyStateWrapper.hpp"
