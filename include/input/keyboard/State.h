#pragma once

#include "../KeyStateWrapper.h"
#include "Key.h"

namespace arch::input::keyboard {

namespace _details {

class State {
public:

	State() noexcept;

	inline u32 operator[](const Key::Value keyCode) noexcept;

private:

	friend struct System;

	u32 _state[512]{};
};

class StateTime {
public:

	using Time = std::chrono::duration<f32>;
	using Clock = std::chrono::high_resolution_clock;

	StateTime() noexcept = default;

	inline Time operator[](const Key::Value keyCode) noexcept;

private:

	friend struct System;

	Time _time[512]{};
	decltype(Clock::now()) _lastUpdate = Clock::now();
};

} // namespace _details

extern _details::State state;
extern _details::StateTime stateTime;

} // namespace arch::input::keyboard

#include "State.hpp"
