#include "KeyState.h"

namespace arch::input {

consteval u32 operator*(KeyState::Value value) {
	return value == KeyState::invalidKey ? 0 : 1u << value;
}

consteval u32 operator+(KeyState::Value lhs, KeyState::Value rhs) {
	return *lhs | *rhs;
}

consteval u32 operator+(u32 lhs, KeyState::Value rhs) {
	return lhs | *rhs;
}

consteval u32 operator+(KeyState::Value lhs, u32 rhs) {
	return *lhs | rhs;
}

} // namespace arch::input
