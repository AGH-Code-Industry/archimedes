#pragma once

#include <chrono>

#include <Mmath.h>

namespace arch::input {

struct KeyState {
	enum Value : u32 {
		invalidKey = (u32)-1,
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

		_changed,
	};
};

consteval u32 operator*(KeyState::Value value);
consteval u32 operator+(KeyState::Value lhs, KeyState::Value rhs);
consteval u32 operator+(u32 lhs, KeyState::Value rhs);
consteval u32 operator+(KeyState::Value lhs, u32 rhs);

} // namespace arch::input

#include "KeyState.hpp"
