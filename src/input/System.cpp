#include <input/Key.h>
#include <input/KeyCode.h>
#include <input/KeyState.h>
#include <input/Mouse.h>
#include <input/MouseButtonCode.h>
#include <input/System.h>

using Clock = std::chrono::high_resolution_clock;

namespace arch::input {

void System::_init(const Ref<Window>& window) noexcept {
	Mouse::_setWindow(window);

	// make GLFW return keymods
	glfwSetInputMode(window->get(), GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	glfwSetKeyCallback(window->get(), _keyboardKeyCallback);
	glfwSetMouseButtonCallback(window->get(), _mouseButtonCallback);
	glfwSetCursorPosCallback(window->get(), _mouseCursorCallback);
	glfwSetScrollCallback(window->get(), _mouseScrollCallback);
}

// if state changed reset time, otherwise add time delta
#define FRAME_BEGIN(dev, which)                                       \
	if (dev::which._state & KeyState::pressed + KeyState::released) { \
		dev::which._time = {};                                        \
	} else {                                                          \
		dev::which._time += dt;                                       \
	}

#define FRAME_BEGIN_KEYBOARD(key) FRAME_BEGIN(Keyboard, key)
#define FRAME_BEGIN_MOUSE(button) FRAME_BEGIN(Mouse, button)

void System::_frameBegin() noexcept {
	static decltype(Clock::now()) _lastUpdate = Clock::now();
	const auto now = Clock::now();
	const auto dt = now - _lastUpdate;
	_lastUpdate = now;

	// thank God for multiline cursor
	{
		FRAME_BEGIN_KEYBOARD(numLock);
		FRAME_BEGIN_KEYBOARD(numDivide)
		FRAME_BEGIN_KEYBOARD(numMultiply)
		FRAME_BEGIN_KEYBOARD(numSubtract)
		FRAME_BEGIN_KEYBOARD(numAdd)
		FRAME_BEGIN_KEYBOARD(numEnter)
		FRAME_BEGIN_KEYBOARD(numDecimal)
		FRAME_BEGIN_KEYBOARD(numZero)
		FRAME_BEGIN_KEYBOARD(numOne)
		FRAME_BEGIN_KEYBOARD(numTwo)
		FRAME_BEGIN_KEYBOARD(numThree)
		FRAME_BEGIN_KEYBOARD(numFour)
		FRAME_BEGIN_KEYBOARD(numFive)
		FRAME_BEGIN_KEYBOARD(numSix)
		FRAME_BEGIN_KEYBOARD(numSeven)
		FRAME_BEGIN_KEYBOARD(numEight)
		FRAME_BEGIN_KEYBOARD(numNine)
		FRAME_BEGIN_KEYBOARD(F1)
		FRAME_BEGIN_KEYBOARD(F2)
		FRAME_BEGIN_KEYBOARD(F3)
		FRAME_BEGIN_KEYBOARD(F4)
		FRAME_BEGIN_KEYBOARD(F5)
		FRAME_BEGIN_KEYBOARD(F6)
		FRAME_BEGIN_KEYBOARD(F7)
		FRAME_BEGIN_KEYBOARD(F8)
		FRAME_BEGIN_KEYBOARD(F9)
		FRAME_BEGIN_KEYBOARD(F10)
		FRAME_BEGIN_KEYBOARD(F11)
		FRAME_BEGIN_KEYBOARD(F12)
		FRAME_BEGIN_KEYBOARD(F13)
		FRAME_BEGIN_KEYBOARD(F14)
		FRAME_BEGIN_KEYBOARD(F15)
		FRAME_BEGIN_KEYBOARD(F16)
		FRAME_BEGIN_KEYBOARD(F17)
		FRAME_BEGIN_KEYBOARD(F18)
		FRAME_BEGIN_KEYBOARD(F19)
		FRAME_BEGIN_KEYBOARD(F20)
		FRAME_BEGIN_KEYBOARD(F21)
		FRAME_BEGIN_KEYBOARD(F22)
		FRAME_BEGIN_KEYBOARD(F23)
		FRAME_BEGIN_KEYBOARD(F24)
		FRAME_BEGIN_KEYBOARD(F25)
		FRAME_BEGIN_KEYBOARD(esc)
		FRAME_BEGIN_KEYBOARD(pageUp)
		FRAME_BEGIN_KEYBOARD(pageDown)
		FRAME_BEGIN_KEYBOARD(home)
		FRAME_BEGIN_KEYBOARD(end)
		FRAME_BEGIN_KEYBOARD(prtSc)
		FRAME_BEGIN_KEYBOARD(insert)
		FRAME_BEGIN_KEYBOARD(del)
		FRAME_BEGIN_KEYBOARD(capsLock)
		FRAME_BEGIN_KEYBOARD(shiftLeft)
		FRAME_BEGIN_KEYBOARD(shiftRight)
		FRAME_BEGIN_KEYBOARD(ctrlLeft)
		FRAME_BEGIN_KEYBOARD(ctrlRight)
		FRAME_BEGIN_KEYBOARD(altLeft)
		FRAME_BEGIN_KEYBOARD(altRight)
		FRAME_BEGIN_KEYBOARD(superLeft)
		FRAME_BEGIN_KEYBOARD(superRight)
		FRAME_BEGIN_KEYBOARD(arrowLeft)
		FRAME_BEGIN_KEYBOARD(arrowRight)
		FRAME_BEGIN_KEYBOARD(arrowDown)
		FRAME_BEGIN_KEYBOARD(arrowUp)
		FRAME_BEGIN_KEYBOARD(menu)
		FRAME_BEGIN_KEYBOARD(zero)
		FRAME_BEGIN_KEYBOARD(one)
		FRAME_BEGIN_KEYBOARD(two)
		FRAME_BEGIN_KEYBOARD(three)
		FRAME_BEGIN_KEYBOARD(four)
		FRAME_BEGIN_KEYBOARD(five)
		FRAME_BEGIN_KEYBOARD(six)
		FRAME_BEGIN_KEYBOARD(seven)
		FRAME_BEGIN_KEYBOARD(eight)
		FRAME_BEGIN_KEYBOARD(nine)
		FRAME_BEGIN_KEYBOARD(A)
		FRAME_BEGIN_KEYBOARD(B)
		FRAME_BEGIN_KEYBOARD(C)
		FRAME_BEGIN_KEYBOARD(D)
		FRAME_BEGIN_KEYBOARD(E)
		FRAME_BEGIN_KEYBOARD(F)
		FRAME_BEGIN_KEYBOARD(G)
		FRAME_BEGIN_KEYBOARD(H)
		FRAME_BEGIN_KEYBOARD(I)
		FRAME_BEGIN_KEYBOARD(J)
		FRAME_BEGIN_KEYBOARD(K)
		FRAME_BEGIN_KEYBOARD(L)
		FRAME_BEGIN_KEYBOARD(M)
		FRAME_BEGIN_KEYBOARD(N)
		FRAME_BEGIN_KEYBOARD(O)
		FRAME_BEGIN_KEYBOARD(P)
		FRAME_BEGIN_KEYBOARD(Q)
		FRAME_BEGIN_KEYBOARD(R)
		FRAME_BEGIN_KEYBOARD(S)
		FRAME_BEGIN_KEYBOARD(T)
		FRAME_BEGIN_KEYBOARD(U)
		FRAME_BEGIN_KEYBOARD(V)
		FRAME_BEGIN_KEYBOARD(W)
		FRAME_BEGIN_KEYBOARD(X)
		FRAME_BEGIN_KEYBOARD(Y)
		FRAME_BEGIN_KEYBOARD(Z)
		FRAME_BEGIN_KEYBOARD(tab)
		FRAME_BEGIN_KEYBOARD(grave)
		FRAME_BEGIN_KEYBOARD(minus)
		FRAME_BEGIN_KEYBOARD(equal)
		FRAME_BEGIN_KEYBOARD(space)
		FRAME_BEGIN_KEYBOARD(backspace)
		FRAME_BEGIN_KEYBOARD(enter)
		FRAME_BEGIN_KEYBOARD(bracketLeft)
		FRAME_BEGIN_KEYBOARD(bracketRight)
		FRAME_BEGIN_KEYBOARD(backSlash)
		FRAME_BEGIN_KEYBOARD(semicolon)
		FRAME_BEGIN_KEYBOARD(apostrophe)
		FRAME_BEGIN_KEYBOARD(comma)
		FRAME_BEGIN_KEYBOARD(dot)
		FRAME_BEGIN_KEYBOARD(slash)
		FRAME_BEGIN_MOUSE(first)
		FRAME_BEGIN_MOUSE(second)
		FRAME_BEGIN_MOUSE(third)
		FRAME_BEGIN_MOUSE(fourth)
		FRAME_BEGIN_MOUSE(fifth)
		FRAME_BEGIN_MOUSE(sixth)
		FRAME_BEGIN_MOUSE(seventh)
		FRAME_BEGIN_MOUSE(eighth)
	}
}

#undef FRAME_BEGIN_KEYBOARD
#undef FRAME_BEGIN_MOUSE

// remove pressed, released and repeat from state
#define FRAME_END(dev, which) dev::which._state &= ~(KeyState::pressed + KeyState::released + KeyState::repeat);
#define FRAME_END_KEYBOARD(key) FRAME_END(Keyboard, key)
#define FRAME_END_MOUSE(button) FRAME_END(Mouse, button)

void System::_frameEnd() noexcept {
	{
		FRAME_END_KEYBOARD(numLock);
		FRAME_END_KEYBOARD(numDivide)
		FRAME_END_KEYBOARD(numMultiply)
		FRAME_END_KEYBOARD(numSubtract)
		FRAME_END_KEYBOARD(numAdd)
		FRAME_END_KEYBOARD(numEnter)
		FRAME_END_KEYBOARD(numDecimal)
		FRAME_END_KEYBOARD(numZero)
		FRAME_END_KEYBOARD(numOne)
		FRAME_END_KEYBOARD(numTwo)
		FRAME_END_KEYBOARD(numThree)
		FRAME_END_KEYBOARD(numFour)
		FRAME_END_KEYBOARD(numFive)
		FRAME_END_KEYBOARD(numSix)
		FRAME_END_KEYBOARD(numSeven)
		FRAME_END_KEYBOARD(numEight)
		FRAME_END_KEYBOARD(numNine)
		FRAME_END_KEYBOARD(F1)
		FRAME_END_KEYBOARD(F2)
		FRAME_END_KEYBOARD(F3)
		FRAME_END_KEYBOARD(F4)
		FRAME_END_KEYBOARD(F5)
		FRAME_END_KEYBOARD(F6)
		FRAME_END_KEYBOARD(F7)
		FRAME_END_KEYBOARD(F8)
		FRAME_END_KEYBOARD(F9)
		FRAME_END_KEYBOARD(F10)
		FRAME_END_KEYBOARD(F11)
		FRAME_END_KEYBOARD(F12)
		FRAME_END_KEYBOARD(F13)
		FRAME_END_KEYBOARD(F14)
		FRAME_END_KEYBOARD(F15)
		FRAME_END_KEYBOARD(F16)
		FRAME_END_KEYBOARD(F17)
		FRAME_END_KEYBOARD(F18)
		FRAME_END_KEYBOARD(F19)
		FRAME_END_KEYBOARD(F20)
		FRAME_END_KEYBOARD(F21)
		FRAME_END_KEYBOARD(F22)
		FRAME_END_KEYBOARD(F23)
		FRAME_END_KEYBOARD(F24)
		FRAME_END_KEYBOARD(F25)
		FRAME_END_KEYBOARD(esc)
		FRAME_END_KEYBOARD(pageUp)
		FRAME_END_KEYBOARD(pageDown)
		FRAME_END_KEYBOARD(home)
		FRAME_END_KEYBOARD(end)
		FRAME_END_KEYBOARD(prtSc)
		FRAME_END_KEYBOARD(insert)
		FRAME_END_KEYBOARD(del)
		FRAME_END_KEYBOARD(capsLock)
		FRAME_END_KEYBOARD(shiftLeft)
		FRAME_END_KEYBOARD(shiftRight)
		FRAME_END_KEYBOARD(ctrlLeft)
		FRAME_END_KEYBOARD(ctrlRight)
		FRAME_END_KEYBOARD(altLeft)
		FRAME_END_KEYBOARD(altRight)
		FRAME_END_KEYBOARD(superLeft)
		FRAME_END_KEYBOARD(superRight)
		FRAME_END_KEYBOARD(arrowLeft)
		FRAME_END_KEYBOARD(arrowRight)
		FRAME_END_KEYBOARD(arrowDown)
		FRAME_END_KEYBOARD(arrowUp)
		FRAME_END_KEYBOARD(menu)
		FRAME_END_KEYBOARD(zero)
		FRAME_END_KEYBOARD(one)
		FRAME_END_KEYBOARD(two)
		FRAME_END_KEYBOARD(three)
		FRAME_END_KEYBOARD(four)
		FRAME_END_KEYBOARD(five)
		FRAME_END_KEYBOARD(six)
		FRAME_END_KEYBOARD(seven)
		FRAME_END_KEYBOARD(eight)
		FRAME_END_KEYBOARD(nine)
		FRAME_END_KEYBOARD(A)
		FRAME_END_KEYBOARD(B)
		FRAME_END_KEYBOARD(C)
		FRAME_END_KEYBOARD(D)
		FRAME_END_KEYBOARD(E)
		FRAME_END_KEYBOARD(F)
		FRAME_END_KEYBOARD(G)
		FRAME_END_KEYBOARD(H)
		FRAME_END_KEYBOARD(I)
		FRAME_END_KEYBOARD(J)
		FRAME_END_KEYBOARD(K)
		FRAME_END_KEYBOARD(L)
		FRAME_END_KEYBOARD(M)
		FRAME_END_KEYBOARD(N)
		FRAME_END_KEYBOARD(O)
		FRAME_END_KEYBOARD(P)
		FRAME_END_KEYBOARD(Q)
		FRAME_END_KEYBOARD(R)
		FRAME_END_KEYBOARD(S)
		FRAME_END_KEYBOARD(T)
		FRAME_END_KEYBOARD(U)
		FRAME_END_KEYBOARD(V)
		FRAME_END_KEYBOARD(W)
		FRAME_END_KEYBOARD(X)
		FRAME_END_KEYBOARD(Y)
		FRAME_END_KEYBOARD(Z)
		FRAME_END_KEYBOARD(tab)
		FRAME_END_KEYBOARD(grave)
		FRAME_END_KEYBOARD(minus)
		FRAME_END_KEYBOARD(equal)
		FRAME_END_KEYBOARD(space)
		FRAME_END_KEYBOARD(backspace)
		FRAME_END_KEYBOARD(enter)
		FRAME_END_KEYBOARD(bracketLeft)
		FRAME_END_KEYBOARD(bracketRight)
		FRAME_END_KEYBOARD(backSlash)
		FRAME_END_KEYBOARD(semicolon)
		FRAME_END_KEYBOARD(apostrophe)
		FRAME_END_KEYBOARD(comma)
		FRAME_END_KEYBOARD(dot)
		FRAME_END_KEYBOARD(slash)
		FRAME_END_MOUSE(first)
		FRAME_END_MOUSE(second)
		FRAME_END_MOUSE(third)
		FRAME_END_MOUSE(fourth)
		FRAME_END_MOUSE(fifth)
		FRAME_END_MOUSE(sixth)
		FRAME_END_MOUSE(seventh)
		FRAME_END_MOUSE(eighth)
	}

	Mouse::_dx = Mouse::_dy = 0;
	Mouse::scroll._x = Mouse::scroll._y = 0;
}

#undef FRAME_END_KEYBOARD
#undef FRAME_END_MOUSE

void System::_keyboardKeyCallback(GLFWwindow* window, int keyCode, int scancode, int action, int mods) noexcept {
	auto&& key = Key::get(keyCode);
	if (action == GLFW_RELEASE) {
		key._state = KeyState::released + KeyState::up;
	} else if (action == GLFW_PRESS) {
		key._state = KeyState::pressed + KeyState::down;
	} else if (action == GLFW_REPEAT) {
		key._state = KeyState::down + KeyState::repeat;
	}

	// add mods
	key._state |= mods << (u32)KeyState::_modBegin;
}

void System::_mouseButtonCallback(GLFWwindow* window, int buttonCode, int action, int mods) noexcept {
	auto&& button = Key::get(buttonCode + (u32)MouseButtonCode::begin);
	if (action == GLFW_RELEASE) {
		button._state = KeyState::released + KeyState::up;
	} else if (action == GLFW_PRESS) {
		button._state = KeyState::pressed + KeyState::down;
	}
	// no repeat for mouse buttons :(

	// add mods
	button._state |= mods << (u32)KeyState::_modBegin;
}

void System::_mouseCursorCallback(GLFWwindow* window, double x, double y) noexcept {
	// invert Y axis
	const double actualY = (double)Mouse::_window->getSize().y - y;

	Mouse::_dx = Mouse::_delta * (x - std::exchange(Mouse::_x, x));
	Mouse::_dy = Mouse::_delta * (actualY - std::exchange(Mouse::_y, actualY));
	Mouse::_delta = true;
}

void System::_mouseScrollCallback(GLFWwindow* window, double x, double y) noexcept {
	Mouse::scroll._x = x;
	Mouse::scroll._y = y;
}

} // namespace arch::input
