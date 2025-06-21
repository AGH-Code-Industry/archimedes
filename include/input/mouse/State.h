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

	inline float x() const noexcept;
	inline float y() const noexcept;
	inline float dx() const noexcept;
	inline float dy() const noexcept;

	inline float scrollX() const noexcept;
	inline float scrollY() const noexcept;

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
	float _x{};
	float _y{};
	float _dx{};
	float _dy{};
	float _xscroll{};
	float _yscroll{};
	bool _delta = false;
	bool _cursorDisabled = false;
	bool _raw = false;
};

extern State state;

} // namespace arch::input::mouse::_details

#include "State.hpp"
