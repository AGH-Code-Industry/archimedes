#pragma once

#include <memory>
#include <string>

#include "TypeDescriptorBlock.h"

namespace arch::meta::rtti {

template<class T>
class TypeDescriptor;

class TypeDescriptorWrapper {
public:
	std::string_view name() const noexcept;
	size_t size() const noexcept;
	size_t alignment() const noexcept;
	size_t hash() const noexcept;

private:
	TypeDescriptorWrapper() = default;
	TypeDescriptorWrapper(TypeDescriptorBlock* block);

	template<class T>
	friend class TypeDescriptor;
	friend class TypeDescriptorBlock;
	friend std::unique_ptr<TypeDescriptorWrapper> std::make_unique<TypeDescriptorWrapper>(TypeDescriptorBlock*&&);

	TypeDescriptorBlock* _block;
};

} // namespace arch::meta::rtti
