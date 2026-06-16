#include <utility>

#include "Key.h"
//
#include "Keyboard.h"
#include "Mouse.h"

#define ARCH_STATE(x)                 \
	bool Key::x() const noexcept {    \
		return _state & *KeyState::x; \
	}
#define ARCH_KEYBOARD_KEY(xx) \
	case (u32)KeyCode::xx: return Keyboard::xx;
#define ARCH_MOUSE_BUTTON(xx) \
	case (u32)MouseButtonCode::xx: return Mouse::xx;

namespace arch::input {

Key::Key() noexcept: _state{ *KeyState::up }, _time{} {}

constexpr Key& Key::get(const u32 code) {
	switch (code) {
		ARCH_KEYBOARD_KEY(numLock)
		ARCH_KEYBOARD_KEY(numDivide)
		ARCH_KEYBOARD_KEY(numMultiply)
		ARCH_KEYBOARD_KEY(numSubtract)
		ARCH_KEYBOARD_KEY(numAdd)
		ARCH_KEYBOARD_KEY(numEnter)
		ARCH_KEYBOARD_KEY(numDecimal)
		ARCH_KEYBOARD_KEY(numZero)
		ARCH_KEYBOARD_KEY(numOne)
		ARCH_KEYBOARD_KEY(numTwo)
		ARCH_KEYBOARD_KEY(numThree)
		ARCH_KEYBOARD_KEY(numFour)
		ARCH_KEYBOARD_KEY(numFive)
		ARCH_KEYBOARD_KEY(numSix)
		ARCH_KEYBOARD_KEY(numSeven)
		ARCH_KEYBOARD_KEY(numEight)
		ARCH_KEYBOARD_KEY(numNine)
		ARCH_KEYBOARD_KEY(F1)
		ARCH_KEYBOARD_KEY(F2)
		ARCH_KEYBOARD_KEY(F3)
		ARCH_KEYBOARD_KEY(F4)
		ARCH_KEYBOARD_KEY(F5)
		ARCH_KEYBOARD_KEY(F6)
		ARCH_KEYBOARD_KEY(F7)
		ARCH_KEYBOARD_KEY(F8)
		ARCH_KEYBOARD_KEY(F9)
		ARCH_KEYBOARD_KEY(F10)
		ARCH_KEYBOARD_KEY(F11)
		ARCH_KEYBOARD_KEY(F12)
		ARCH_KEYBOARD_KEY(F13)
		ARCH_KEYBOARD_KEY(F14)
		ARCH_KEYBOARD_KEY(F15)
		ARCH_KEYBOARD_KEY(F16)
		ARCH_KEYBOARD_KEY(F17)
		ARCH_KEYBOARD_KEY(F18)
		ARCH_KEYBOARD_KEY(F19)
		ARCH_KEYBOARD_KEY(F20)
		ARCH_KEYBOARD_KEY(F21)
		ARCH_KEYBOARD_KEY(F22)
		ARCH_KEYBOARD_KEY(F23)
		ARCH_KEYBOARD_KEY(F24)
		ARCH_KEYBOARD_KEY(F25)
		ARCH_KEYBOARD_KEY(esc)
		ARCH_KEYBOARD_KEY(pageUp)
		ARCH_KEYBOARD_KEY(pageDown)
		ARCH_KEYBOARD_KEY(home)
		ARCH_KEYBOARD_KEY(end)
		ARCH_KEYBOARD_KEY(prtSc)
		ARCH_KEYBOARD_KEY(insert)
		ARCH_KEYBOARD_KEY(del)
		ARCH_KEYBOARD_KEY(capsLock)
		ARCH_KEYBOARD_KEY(shiftLeft)
		ARCH_KEYBOARD_KEY(shiftRight)
		ARCH_KEYBOARD_KEY(ctrlLeft)
		ARCH_KEYBOARD_KEY(ctrlRight)
		ARCH_KEYBOARD_KEY(altLeft)
		ARCH_KEYBOARD_KEY(altRight)
		ARCH_KEYBOARD_KEY(superLeft)
		ARCH_KEYBOARD_KEY(superRight)
		ARCH_KEYBOARD_KEY(arrowLeft)
		ARCH_KEYBOARD_KEY(arrowRight)
		ARCH_KEYBOARD_KEY(arrowDown)
		ARCH_KEYBOARD_KEY(arrowUp)
		ARCH_KEYBOARD_KEY(menu)
		ARCH_KEYBOARD_KEY(zero)
		ARCH_KEYBOARD_KEY(one)
		ARCH_KEYBOARD_KEY(two)
		ARCH_KEYBOARD_KEY(three)
		ARCH_KEYBOARD_KEY(four)
		ARCH_KEYBOARD_KEY(five)
		ARCH_KEYBOARD_KEY(six)
		ARCH_KEYBOARD_KEY(seven)
		ARCH_KEYBOARD_KEY(eight)
		ARCH_KEYBOARD_KEY(nine)
		ARCH_KEYBOARD_KEY(A)
		ARCH_KEYBOARD_KEY(B)
		ARCH_KEYBOARD_KEY(C)
		ARCH_KEYBOARD_KEY(D)
		ARCH_KEYBOARD_KEY(E)
		ARCH_KEYBOARD_KEY(F)
		ARCH_KEYBOARD_KEY(G)
		ARCH_KEYBOARD_KEY(H)
		ARCH_KEYBOARD_KEY(I)
		ARCH_KEYBOARD_KEY(J)
		ARCH_KEYBOARD_KEY(K)
		ARCH_KEYBOARD_KEY(L)
		ARCH_KEYBOARD_KEY(M)
		ARCH_KEYBOARD_KEY(N)
		ARCH_KEYBOARD_KEY(O)
		ARCH_KEYBOARD_KEY(P)
		ARCH_KEYBOARD_KEY(Q)
		ARCH_KEYBOARD_KEY(R)
		ARCH_KEYBOARD_KEY(S)
		ARCH_KEYBOARD_KEY(T)
		ARCH_KEYBOARD_KEY(U)
		ARCH_KEYBOARD_KEY(V)
		ARCH_KEYBOARD_KEY(W)
		ARCH_KEYBOARD_KEY(X)
		ARCH_KEYBOARD_KEY(Y)
		ARCH_KEYBOARD_KEY(Z)
		ARCH_KEYBOARD_KEY(tab)
		ARCH_KEYBOARD_KEY(grave)
		ARCH_KEYBOARD_KEY(minus)
		ARCH_KEYBOARD_KEY(equal)
		ARCH_KEYBOARD_KEY(space)
		ARCH_KEYBOARD_KEY(backspace)
		ARCH_KEYBOARD_KEY(enter)
		ARCH_KEYBOARD_KEY(bracketLeft)
		ARCH_KEYBOARD_KEY(bracketRight)
		ARCH_KEYBOARD_KEY(backSlash)
		ARCH_KEYBOARD_KEY(semicolon)
		ARCH_KEYBOARD_KEY(apostrophe)
		ARCH_KEYBOARD_KEY(comma)
		ARCH_KEYBOARD_KEY(dot)
		ARCH_KEYBOARD_KEY(slash)
		ARCH_MOUSE_BUTTON(first)
		ARCH_MOUSE_BUTTON(second)
		ARCH_MOUSE_BUTTON(third)
		ARCH_MOUSE_BUTTON(fourth)
		ARCH_MOUSE_BUTTON(fifth)
		ARCH_MOUSE_BUTTON(sixth)
		ARCH_MOUSE_BUTTON(seventh)
		ARCH_MOUSE_BUTTON(eighth)
	}

	throw std::invalid_argument(std::format("Code {} does not match any key or button", code));
}

Key::Time Key::upTime() const noexcept {
	return _time * up();
}

Key::Time Key::downTime() const noexcept {
	return _time * down();
}

bool Key::changed() const noexcept {
	return _state & KeyState::pressed + KeyState::released;
}

bool Key::has(const KeyState keyState) const noexcept {
	return _state & (1 << (u32)keyState);
}

bool Key::has(const u32 mask) const noexcept {
	return (_state & mask) == mask;
}

bool Key::up() const noexcept {
	return _state & *KeyState::up;
}

ARCH_STATE(released)
ARCH_STATE(down)
ARCH_STATE(pressed)
ARCH_STATE(repeat)
ARCH_STATE(shift)
ARCH_STATE(ctrl)
ARCH_STATE(control)
ARCH_STATE(alt)
ARCH_STATE(super)
ARCH_STATE(win)
ARCH_STATE(windows)
ARCH_STATE(command)
ARCH_STATE(meta)
ARCH_STATE(capsLock)
ARCH_STATE(numLock)

} // namespace arch::input

#undef ARCH_KEYBOARD_KEY
#undef ARCH_MOUSE_BUTTON
#undef ARCH_STATE
