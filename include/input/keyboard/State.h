#pragma once

#include "../KeyStateWrapper.h"
#include "Key.h"

namespace arch::input::keyboard::_details {

/// @brief Class for managing keyboard state
class State {
public:

	/// @brief Default constructor
	State() noexcept;

	/// @brief Returns state of given key
	/// @param keyCode - key to get
	inline KeyStateWrapper operator[](const u32 keyCode) noexcept;

private:

	friend struct System;

	using Time = std::chrono::duration<f32>;
	using Clock = std::chrono::high_resolution_clock;

	u32 _state[512]{};
	Time _time[512]{};
	decltype(Clock::now()) _lastUpdate = Clock::now();
};

/// @brief Object for managing keyboard state
extern State state;

} // namespace arch::input::keyboard::_details

#include "State.hpp"
