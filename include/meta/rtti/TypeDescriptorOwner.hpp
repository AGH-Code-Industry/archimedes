#pragma once

#include "HashTypeName.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorOwner.h"
#include <meta/nameOfType.h>

namespace arch::meta::rtti {

template<class T>
inline const TypeDescriptor& arch::meta::rtti::TypeDescriptorOwner<T>::get() noexcept {
	if (_desc.name.empty()) { // descriptor was not yet initialized
		_desc.name = nameOfType(T);
		_desc.size = sizeof(T);
		_desc.alignment = alignof(T);
		_desc.hash = hashTypeName(_desc.name);
	}

	return _desc;
}

} // namespace arch::meta::rtti
