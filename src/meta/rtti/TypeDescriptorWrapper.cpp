#include <archimedes/meta/rtti/TypeDescriptorWrapper.h>

namespace arch::meta::rtti {

TypeDescriptorWrapper::TypeDescriptorWrapper(const TypeDescriptor& desc) noexcept: _desc{ &desc } {}

TypeDescriptorWrapper& arch::meta::rtti::TypeDescriptorWrapper::operator=(const TypeDescriptor& desc) noexcept {
	_desc = &desc;

	return *this;
}

bool TypeDescriptorWrapper::operator==(const TypeDescriptorWrapper& other) const noexcept {
	return get() == other.get();
}

std::strong_ordering TypeDescriptorWrapper::operator<=>(const TypeDescriptorWrapper& other) const noexcept {
	return get() <=> other.get();
}

const TypeDescriptor& TypeDescriptorWrapper::get() const noexcept {
	return *_desc;
}

const TypeDescriptor& TypeDescriptorWrapper::operator*() const noexcept {
	return *_desc;
}

std::string_view TypeDescriptorWrapper::name() const noexcept {
	return _desc->name;
}

size_t TypeDescriptorWrapper::size() const noexcept {
	return _desc->size;
}

size_t TypeDescriptorWrapper::alignment() const noexcept {
	return _desc->alignment;
}

size_t TypeDescriptorWrapper::hash() const noexcept {
	return _desc->hash;
}

} // namespace arch::meta::rtti
