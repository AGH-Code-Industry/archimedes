#include "Typedesc.h"
//
#include "RTTIEnabled.hpp"

#include "TypeDescriptorOwner.h"

namespace arch::meta::rtti {

template<class T>
const TypeDescriptor& typedesc() noexcept {
	return TypeDescriptorOwner<std::remove_cvref_t<T>>::get();
}

template<class T>
requires(RTTIEnabled<T>)
const TypeDescriptor& typedesc(const T& obj) noexcept {
	return obj._getTypeDescriptor();
}

template<class T>
requires(!RTTIEnabled<T>)
const TypeDescriptor& typedesc(const T& obj) noexcept {
	return typedesc<T>();
}

} // namespace arch::meta::rtti
