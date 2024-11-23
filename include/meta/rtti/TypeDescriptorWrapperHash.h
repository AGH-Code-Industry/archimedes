#pragma once

#include "TypeDescriptor.h"

/// @brief Specialization of std::hash, enabling usage of TypeDescriptorWrapper as key in hash containers
template<>
struct std::hash<arch::meta::rtti::TypeDescriptorWrapper> {
public:
	/// @brief Default constructor.
	hash() = default;
	/// @brief Copy constructor.
	hash(const hash&) = default;
	/// @brief Move constructor.
	hash(hash&&) = default;

	/// @brief Copy-assignment operator.
	hash& operator=(const hash&) = default;
	/// @brief Move-assignment operator.
	hash& operator=(hash&&) = default;

	/// @brief Call operator
	/// @param desc - TypeDescriptorWrapper to hash
	/// @return desc.hash()
	size_t operator()(const arch::meta::rtti::TypeDescriptorWrapper& desc) const noexcept;
};

// #include "TypeDesciptorWrapperHash.hpp"
