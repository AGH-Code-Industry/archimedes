#include "State.h"

namespace arch::input::keyboard::_details {

KeyStateWrapper State::operator()(const Key keyCode) noexcept {
	return { keyCode < 512 ? _state[keyCode] : 0 };
}

KeyStateWrapper State::operator[](const Key keyCode) noexcept {
	return (*this)(keyCode);
}

StateTime::Time StateTime::operator()(const Key keyCode) noexcept {
	return { keyCode < 512 ? _time[keyCode] : invalid };
}

StateTime::Time StateTime::operator[](const Key keyCode) noexcept {
	return (*this)(keyCode);
}

} // namespace arch::input::keyboard::_details
