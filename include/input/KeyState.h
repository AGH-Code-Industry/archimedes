#pragma once

#include <chrono>

#include <Mmath.h>

namespace arch::input {

/// @brief Enum with all key/button states
struct KeyState {
	enum Value : u32 {
		up = 0,
		released,
		down,
		pressed,
		repeat,

		shift,
		ctrl,
		control = ctrl,
		alt,
		super,
		win = super,
		windows = win,
		command = super,
		meta = super,
		capsLock,
		numLock,
	};
};

/// @brief Helper operator returning bitmask of given KeyState
/// @param value - KeyState to get bitmask of
consteval u32 operator*(KeyState::Value value);
/// @brief Helper operator combining bitmasks of given KeyStates
/// @param lhs - first KeyState
/// @param rhs - second KeyState
consteval u32 operator+(KeyState::Value lhs, KeyState::Value rhs);
/// @brief Helper operator combining bitmask with given KeyState
/// @param lhs - bitmask
/// @param rhs - second KeyState
consteval u32 operator+(u32 lhs, KeyState::Value rhs);

} // namespace arch::input

#include "KeyState.hpp"
