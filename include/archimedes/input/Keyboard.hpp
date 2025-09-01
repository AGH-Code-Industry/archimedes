#include "Key.h"
#include "Keyboard.h"

namespace arch::input {

constexpr Key& Keyboard::key(const KeyCode code) {
	return Key::get((u32)code);
}

} // namespace arch::input
