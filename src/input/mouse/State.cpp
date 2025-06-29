#include <input/mouse/State.h>

namespace arch::input::mouse::_details {

void State::_init(const Ref<Window>& window) noexcept {
	for (auto&& s : _state) {
		s = *KeyState::up;
	}

	_window = window;
}

State state{};

} // namespace arch::input::mouse::_details
