#pragma once

#include <memory>
#include <string>

#include "TypeDescriptor.h"

namespace arch::meta::rtti {
/// @brief Templated owner of TypeDescriptor
/// @tparam T - type which descriptor this class holds
template<class T>
class TypeDescriptorOwner {
public:
	static const TypeDescriptor& get() noexcept;

private:
	static inline TypeDescriptor _desc;
};
} // namespace arch::meta::rtti

#include "TypeDescriptorOwner.hpp"
