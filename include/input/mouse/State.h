#pragma once

#include "../KeyStateWrapper.h"
#include "Button.h"
#include <GLFW/glfw3.h>
#include <Ref.h>
#include <Window.h>

namespace arch::input::mouse::_details {

/// @brief Class for managing mouse state
class State {
public:

	/// @brief Default constructor
	State() noexcept = default;

	/// @brief Returns state of given button
	/// @param buttonCode - button to get
	inline KeyStateWrapper operator[](const u32 buttonCode) noexcept;

	/// @brief Returns mouse position on X axis
	inline double x() const noexcept;
	/// @brief Returns mouse position on Y axis
	inline double y() const noexcept;
	/// @brief Returns change of mouse position on X axis
	inline double dx() const noexcept;
	/// @brief Returns change of mouse position on Y axis
	inline double dy() const noexcept;

	/// @brief Returns scroll speed on X axis (less common)
	inline double scrollX() const noexcept;
	/// @brief Returns scroll speed on Y axis (most common)
	inline double scrollY() const noexcept;

	/// @brief Sets whether cursor should be disabled: hidden and locked to current window, best for camera control
	/// @param value - whether to disable cursor
	inline void cursorDisabled(bool value) noexcept;
	/// @brief Returns whether cursor is disabled
	inline bool cursorDisabled() const noexcept;

	/// @brief Sets whether to use raw (unscaled and unaccelerated) mouse motion
	/// @details Available only if cursor is disabled and supportsRaw() returns true
	/// @param value - whether to enable raw mouse motion
	inline void raw(bool value) noexcept;
	/// @brief Returns whether raw mouse motion is enabled
	inline bool raw() const noexcept;
	/// @brief Returns whether raw mouse motion is supported
	inline bool supportsRaw() const noexcept;

private:

	friend struct System;

	using Time = std::chrono::duration<f32>;
	using Clock = std::chrono::high_resolution_clock;

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

/// @brief Object for managing mouse state
extern State state;

} // namespace arch::input::mouse::_details

#include "State.hpp"
