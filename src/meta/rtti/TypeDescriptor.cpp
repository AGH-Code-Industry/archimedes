#include <meta/rtti/TypeDescriptor.h>

namespace arch::meta::rtti {
bool TypeDescriptor::operator==(const TypeDescriptor& other) const noexcept {
	return this == (&other);
}

std::strong_ordering TypeDescriptor::operator<=>(const TypeDescriptor& other) const noexcept {
	return name <=> other.name;
}
} // namespace arch::meta::rtti
