#include "Key.h"
#include "Mouse.h"

namespace arch::input {

constexpr Key& Mouse::button(const MouseButtonCode code) {
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

double Mouse::Scroll::x() const noexcept {
	return _x;
}

double Mouse::Scroll::y() const noexcept {
	return _y;
}

bool Mouse::Cursor::disabled() const noexcept {
	return _disabled;
}

void Mouse::Cursor::toggle(bool toggleRaw) noexcept {
	disabled(!disabled(), toggleRaw);
}

bool Mouse::RawInput::enabled() const noexcept {
	return _enabled;
}

void Mouse::RawInput::toggle() noexcept {
	enabled(!enabled());
}

} // namespace arch::input
