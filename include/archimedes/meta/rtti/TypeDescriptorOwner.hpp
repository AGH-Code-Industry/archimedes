#pragma once

#include "HashTypeName.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorOwner.h"
#include <archimedes/meta/NameOfType.h>

namespace arch::meta::rtti {

template<class T>
inline const TypeDescriptor& arch::meta::rtti::TypeDescriptorOwner<T>::get() noexcept {
	if (_desc.name.empty()) { // descriptor was not yet initialized
		_desc.name = nameOfType(T);
		_desc.size = sizeof(T);
		_desc.alignment = alignof(T);
		_desc.hash = hashTypeName(_desc.name);
		_desc._typeid = &typeid(T);
	}

	return _desc;
}

template<>
inline const TypeDescriptor& arch::meta::rtti::TypeDescriptorOwner<void>::get() noexcept {
	// sizeof and alignof does not work for void
	if (_desc.name.empty()) { // descriptor was not yet initialized
		_desc.name = nameOfType(void);
		_desc.size = 0;
		_desc.alignment = 0;
		_desc.hash = hashTypeName(_desc.name);
		_desc._typeid = &typeid(void);
	}

	return _desc;
}

} // namespace arch::meta::rtti
