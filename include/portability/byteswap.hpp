#pragma once

namespace arch::port {
/// @brief Reverses byte order in given object.
/// @param obj - object to reverse
/// @return Reversed object.
template<class T>
T& byteswap(T& obj) noexcept {
	unsigned char temp;
	for (size_t i = 0; i != sizeof(T) / 2; ++i) {
		temp = *((unsigned char*)&obj + i);
		*((unsigned char*)&obj + i) = *((unsigned char*)&obj + sizeof(T) - 1 - i);
		*((unsigned char*)&obj + sizeof(T) - 1 - i) = temp;
	}
	return obj;
}
}