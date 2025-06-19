#pragma once

#include "../KeyStateWrapper.h"
#include "Key.h"

namespace arch::input::keyboard::_details {

class State {
public:

	State() noexcept;

	inline u32 operator[](const Key::Code keyCode) noexcept;

private:

	friend struct System;

	u32 _state[512]{};
};

class StateTime {
public:

	using Time = std::chrono::duration<f32>;
	using Clock = std::chrono::high_resolution_clock;

	StateTime() noexcept = default;

	inline Time operator[](const Key::Code keyCode) noexcept;

private:

	friend struct System;

	Time _time[512]{};
	decltype(Clock::now()) _lastUpdate = Clock::now();
};

extern State state;
extern StateTime stateTime;

} // namespace arch::input::keyboard::_details

#include "State.hpp"
