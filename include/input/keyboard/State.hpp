#include "State.h"

namespace arch::input::keyboard::_details {

KeyStateWrapper State::operator[](const u32 keyCode) noexcept {
	return KeyStateWrapper{ .value = _state[keyCode], .time = _time[keyCode] };
}

} // namespace arch::input::keyboard::_details
