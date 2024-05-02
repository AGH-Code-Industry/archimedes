#include <meta/rtti/TypeDescriptor.h>
#include <meta/rtti/TypeDescriptorWrapper.h>

namespace arch::meta::rtti {
bool TypeDescriptor::operator==(const TypeDescriptor& other) const noexcept {
	return this == &other;
}

bool TypeDescriptor::operator==(const std::type_info& other) const noexcept {
	return *_typeid == other;
}

std::strong_ordering TypeDescriptor::operator<=>(const TypeDescriptor& other) const noexcept {
	if (*this == other) {
		return std::strong_ordering::equal;
	}
	return name <=> other.name;
}

TypeDescriptorWrapper TypeDescriptor::wrap() const noexcept {
	return TypeDescriptorWrapper(*this);
}
} // namespace arch::meta::rtti
