#pragma once

#include "TypeDescriptorHash.h"

size_t std::hash<arch::meta::rtti::TypeDescriptor>::operator()(const arch::meta::rtti::TypeDescriptor& desc
) const noexcept {
	return desc.hash;
}
