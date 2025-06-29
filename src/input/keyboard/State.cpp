#include <input/keyboard/State.h>

namespace arch::input::keyboard::_details {

State::State() noexcept {
	for (auto&& s : _state) {
		s = *KeyState::up;
	}
}

State state{};

} // namespace arch::input::keyboard::_details
