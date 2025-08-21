#pragma once

#include "RTTIEnabled.hpp"
#include <concepts>

#include "TypeDescriptorWrapper.h"

namespace arch::meta::rtti {

template<class T>
const TypeDescriptor& typedesc() noexcept;

template<class T>
requires(RTTIEnabled<T>)
const TypeDescriptor& typedesc(const T& obj) noexcept;

template<class T>
requires(!RTTIEnabled<T>)
const TypeDescriptor& typedesc(const T& obj) noexcept;

} // namespace arch::meta::rtti

namespace arch {

using meta::rtti::typedesc;

}

#include "Typedesc.hpp"
