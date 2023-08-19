#include <serialization/hash.hpp>

size_t arch::ser::hash(const std::string& name) noexcept {
	size_t a = 73;
	size_t p = 1e9 + 9;

	size_t to_return{};
	for (size_t i = 0; i != name.length(); ++i) {
		to_return = ((to_return * a) + name[i]) % p;
	}
	return to_return;
}