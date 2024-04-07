#include <meta/rtti/HashTypeName.h>

namespace arch::meta::rtti {
size_t hashTypeName(std::string_view typeName) noexcept {
	constexpr size_t m = (1ull << 61ull) - 1ull;
	constexpr size_t p = 101;

	size_t result = 0;

	for (size_t i = 0; i != typeName.length(); ++i) {
		result ^= typeName[i];
		result *= p;
		result %= m;
	}

	return result;
}
} // namespace arch::meta::rtti
