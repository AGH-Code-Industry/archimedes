#pragma once

#include "HashTypeName.h"
#include "TypeDescriptor.h"
#include "TypeDescriptorBlock.h"
#include "TypeDescriptorWrapper.h"
#include <meta/nameOfType.h>

namespace arch::meta::rtti {

template<class T>
inline const TypeDescriptorWrapper& arch::meta::rtti::TypeDescriptor<T>::get() noexcept {
	if (not _wrapper) { // descriptor was not yet initialized
		_block.name = nameOfType(T);
		_block.size = sizeof(T);
		_block.alignment = alignof(T);
		_block.hash = hashTypeName(_block.name);

		_wrapper = std::make_unique<TypeDescriptorWrapper>(&_block);
	}

	return *_wrapper;
}

} // namespace arch::meta::rtti
