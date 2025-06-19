#include "State.h"

namespace arch::input::keyboard::_details {

u32 State::operator[](const Key::Code keyCode) noexcept {
	return _state[keyCode];
}

StateTime::Time StateTime::operator[](const Key::Code keyCode) noexcept {
	return _time[keyCode];
}

} // namespace arch::input::keyboard::_details
