#pragma once

#include "State.h"

namespace arch::input::mouse {

/// @brief Returns state of given button
/// @param buttonCode - button to get
inline KeyStateWrapper button(const u32 buttonCode) noexcept;

/// @brief Returns mouse position on X axis
inline double x() noexcept;
/// @brief Returns mouse position on Y axis
inline double y() noexcept;
/// @brief Returns mouse position
inline double2 pos() noexcept;
/// @brief Returns change of mouse position on X axis
inline double dx() noexcept;
/// @brief Returns change of mouse position on Y axis
inline double dy() noexcept;
/// @brief Returns change of mouse position
inline double2 dpos() noexcept;

namespace scroll {

/// @brief Returns scroll speed on X axis (less common)
inline double x() noexcept;
/// @brief Returns scroll speed on Y axis (most common)
inline double y() noexcept;

} // namespace scroll

namespace cursor {

/// @brief Sets whether cursor should be disabled: hidden and locked to current window, best for camera control
/// @param value - whether to disable cursor
/// @param setRaw (optional) - whether to set raw mouse input, default value true
inline void disabled(bool value, bool setRaw = true) noexcept;
/// @brief Returns whether cursor is disabled
inline bool disabled() noexcept;
/// @brief Toggles whether cursor should be disabled
/// @param toggleRaw (optional) - whether to set raw mouse input when disabling cursor
inline void toggle(bool toggleRaw = true) noexcept;

} // namespace cursor

/// @brief Sets whether to use raw (unscaled and unaccelerated) mouse motion
/// @details Available only if cursor is disabled and supportsRaw() returns true
/// @param value - whether to enable raw mouse motion
inline void rawInput(bool value) noexcept;
/// @brief Toggles rawInput
inline void rawInputToggle() noexcept;
/// @brief Returns whether raw mouse motion is enabled
inline bool rawInput() noexcept;
/// @brief Returns whether raw mouse motion is supported
inline bool supportsRaw() noexcept;

inline KeyStateWrapper left() noexcept;
inline KeyStateWrapper right() noexcept;
inline KeyStateWrapper middle() noexcept;
inline KeyStateWrapper first() noexcept;
inline KeyStateWrapper second() noexcept;
inline KeyStateWrapper third() noexcept;
inline KeyStateWrapper fourth() noexcept;
inline KeyStateWrapper fifth() noexcept;
inline KeyStateWrapper sixth() noexcept;
inline KeyStateWrapper seventh() noexcept;
inline KeyStateWrapper eighth() noexcept;

} // namespace arch::input::mouse

#include "Accessors.hpp"
