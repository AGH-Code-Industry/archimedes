#include <meta/rtti/TypeDescriptor.h>

namespace arch::meta::rtti {
bool TypeDescriptor::operator==(const TypeDescriptor& other) const noexcept {
	if (hash == other.hash and size == other.size and alignment == other.alignment) {
		return name == other.name;
	}
	return false;
}

std::strong_ordering TypeDescriptor::operator<=>(const TypeDescriptor& other) const noexcept {
	return name <=> other.name;
}
} // namespace arch::meta::rtti
