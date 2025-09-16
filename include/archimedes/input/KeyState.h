#pragma once

#include <archimedes/Mmath.h>

namespace arch::input {

/// @brief Enum with all key/button states
enum class KeyState : u32 {
	up = 0,
	released,
	down,
	pressed,
	repeat,

	_modBegin,
	shift = _modBegin,
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

/// @brief Helper operator returning bitmask of given KeyState
/// @param value - KeyState to get bitmask of
consteval u32 operator*(KeyState value);
/// @brief Helper operator combining bitmasks of given KeyStates
/// @param lhs - first KeyState
/// @param rhs - second KeyState
consteval u32 operator+(KeyState lhs, KeyState rhs);
/// @brief Helper operator combining bitmask with given KeyState
/// @param lhs - bitmask
/// @param rhs - second KeyState
consteval u32 operator+(u32 lhs, KeyState rhs);

} // namespace arch::input

#include "KeyState.hpp"
