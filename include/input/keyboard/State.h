#pragma once

#include "../KeyStateWrapper.h"
#include "Key.h"

namespace arch::input::keyboard::_details {

class State {
public:

	using Time = std::chrono::duration<f32>;
	using Clock = std::chrono::high_resolution_clock;

	State() noexcept;

private:

	friend struct System;

	u32 _state[512]{};
	Time _time[512]{};
	decltype(Clock::now()) _lastUpdate = Clock::now();
};

extern State state;

} // namespace arch::input::keyboard::_details
