#pragma once

#include "State.h"

namespace arch::input::mouse {

inline KeyStateWrapper button(const Button::Code buttonCode) noexcept;

inline float x() noexcept;
inline float y() noexcept;
inline float2 pos() noexcept;
inline float dx() noexcept;
inline float dy() noexcept;
inline float2 dpos() noexcept;

namespace scroll {

inline float x() noexcept;
inline float y() noexcept;

} // namespace scroll

namespace cursor {

inline void disabled(bool value, bool setRaw = true) noexcept;
inline bool disabled() noexcept;
inline void toggle(bool toggleRaw = true) noexcept;

} // namespace cursor

inline void rawInput(bool value) noexcept;
inline void rawInputToggle() noexcept;
inline bool rawInput() noexcept;
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
