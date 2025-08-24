#include <archimedes/meta/rtti/TypeDescriptorWrapper.h>
#include <archimedes/meta/rtti/TypeDescriptorWrapperHash.h>

size_t std::hash<arch::meta::rtti::TypeDescriptorWrapper>::operator()(
	const arch::meta::rtti::TypeDescriptorWrapper& desc
) const noexcept {
	return desc.hash();
}
