#pragma once

#include <string>

namespace arch::meta::rtti {
struct TypeDescriptorBlock {
	std::string name;
	size_t size;
	size_t alignment;
	size_t hash;
};
} // namespace arch::meta::rtti
