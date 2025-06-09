#pragma once

#include "../KeyStateWrapper.h"
#include "Key.h"
#include <Defines.h>

namespace arch::input::keyboard {

namespace _details {

class State {
public:

	State() noexcept = default;

	inline KeyStateWrapper operator[](const Key keyCode) noexcept;
	inline KeyStateWrapper operator()(const Key keyCode) noexcept;

	static constexpr KeyStateWrapper invalid = KeyStateWrapper{ 0 };

private:

	friend struct System;

	u32 _state[512]{};
};

class StateTime {
public:

	using Time = std::chrono::duration<f32>;
	using Clock = std::chrono::high_resolution_clock;

	StateTime() noexcept = default;

	inline Time operator[](const Key keyCode) noexcept;
	inline Time operator()(const Key keyCode) noexcept;

	static constexpr Time invalid = Time(-1);

private:

	friend struct System;

	Time _time[512]{};
	decltype(Clock::now()) _lastUpdate;
};

} // namespace _details

extern _details::State state;
extern _details::StateTime stateTime;

} // namespace arch::input::keyboard

#include "State.hpp"
