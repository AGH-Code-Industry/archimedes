#include "Key.h"
#include "Mouse.h"

namespace arch::input {

constexpr Key& Mouse::button(const MouseButtonCode code) noexcept {
	return Key::get((u32)code);
}

double Mouse::x() noexcept {
	return Mouse::_x;
}

double Mouse::y() noexcept {
	return Mouse::_y;
}

double2 Mouse::pos() noexcept {
	return { x(), y() };
}

double Mouse::dx() noexcept {
	return Mouse::_dx;
}

double Mouse::dy() noexcept {
	return Mouse::_dy;
}

double2 Mouse::dpos() noexcept {
	return { dx(), dy() };
}

double Mouse::Scroll::x() noexcept {
	return Mouse::_scrollx;
}

double Mouse::Scroll::y() noexcept {
	return Mouse::_scrolly;
}

bool Mouse::Cursor::disabled() noexcept {
	return Mouse::_cursorDisabled;
}

void Mouse::Cursor::toggle(bool toggleRaw) noexcept {
	disabled(!disabled(), toggleRaw);
}

bool Mouse::RawInput::enabled() noexcept {
	return Mouse::_rawInput;
}

void Mouse::RawInput::toggle() noexcept {
	enabled(!enabled());
}

} // namespace arch::input
