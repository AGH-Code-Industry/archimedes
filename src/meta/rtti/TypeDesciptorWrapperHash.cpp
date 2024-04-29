#include <meta/rtti/TypeDescriptorWrapper.h>
#include <meta/rtti/TypeDescriptorWrapperHash.h>

size_t std::hash<arch::meta::rtti::TypeDescriptorWrapper>::operator()(
	const arch::meta::rtti::TypeDescriptorWrapper& desc
) const noexcept {
	return desc.hash();
}
