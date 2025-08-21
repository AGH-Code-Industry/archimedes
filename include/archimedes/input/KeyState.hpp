#include "KeyState.h"

namespace arch::input {

consteval u32 operator*(KeyState value) {
	return 1u << (u32)value;
}

consteval u32 operator+(KeyState lhs, KeyState rhs) {
	return *lhs | *rhs;
}

consteval u32 operator+(u32 lhs, KeyState rhs) {
	return lhs | *rhs;
}

} // namespace arch::input
