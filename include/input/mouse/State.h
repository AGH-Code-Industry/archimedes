#pragma once

#include "../KeyStateWrapper.h"
#include "Button.h"
#include <GLFW/glfw3.h>
#include <Ref.h>
#include <Window.h>

namespace arch::input::mouse::_details {

class State {
public:

	using Time = std::chrono::duration<f32>;
	using Clock = std::chrono::high_resolution_clock;

	State() noexcept = default;

	inline KeyStateWrapper operator[](const u32 buttonCode) noexcept;

	inline double x() const noexcept;
	inline double y() const noexcept;
	inline double dx() const noexcept;
	inline double dy() const noexcept;

	inline double scrollX() const noexcept;
	inline double scrollY() const noexcept;

	inline void cursorDisabled(bool value) noexcept;
	inline bool cursorDisabled() const noexcept;

	inline void raw(bool value) noexcept;
	inline bool raw() const noexcept;
	inline bool supportsRaw() const noexcept;

private:

	friend struct System;

	void _init(const Ref<Window>& window) noexcept;

	u32 _state[8]{};
	Time _time[8]{};
	decltype(Clock::now()) _lastUpdate = Clock::now();
	Ref<Window> _window;
	double _x{};
	double _y{};
	double _dx{};
	double _dy{};
	double _xscroll{};
	double _yscroll{};
	bool _delta = false;
	bool _cursorDisabled = false;
	bool _raw = false;
};

extern State state;

} // namespace arch::input::mouse::_details

#include "State.hpp"
