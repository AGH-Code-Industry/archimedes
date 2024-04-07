#pragma once

#include <memory>
#include <string>

#include "TypeDescriptorBlock.h"

namespace arch::meta::rtti {
class TypeDescriptorWrapper;

template<class T>
class TypeDescriptor {
public:
	static const TypeDescriptorWrapper& get() noexcept;

private:
	friend class TypeDescriptorWrapper;

	static inline TypeDescriptorBlock _block;
	static inline std::unique_ptr<TypeDescriptorWrapper> _wrapper;
};
} // namespace arch::meta::rtti

#include "TypeDescriptor.hpp"
