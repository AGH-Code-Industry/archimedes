#include <input/keyboard/State.h>

namespace arch::input::keyboard {

_details::State::State() noexcept {
	for (auto&& s : _state) {
		s = *KeyState::up;
	}
}

_details::State arch::input::keyboard::state{};
_details::StateTime arch::input::keyboard::stateTime{};

} // namespace arch::input::keyboard
