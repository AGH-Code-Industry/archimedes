#include <meta/rtti/TypeDescriptorWrapper.h>

namespace arch::meta::rtti {
TypeDescriptorWrapper::TypeDescriptorWrapper(TypeDescriptorBlock* block): _block{ block } {}

std::string_view TypeDescriptorWrapper::name() const noexcept {
	return _block->name;
}

size_t TypeDescriptorWrapper::size() const noexcept {
	return _block->size;
}

size_t TypeDescriptorWrapper::alignment() const noexcept {
	return _block->alignment;
}

size_t TypeDescriptorWrapper::hash() const noexcept {
	return _block->hash;
}
} // namespace arch::meta::rtti

