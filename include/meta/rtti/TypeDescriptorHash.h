#pragma once

#include "TypeDescriptor.h"

/// @brief Specialization of std::hash, enabling usage of TypeDescriptor as key in hash containers
template<>
struct std::hash<arch::meta::rtti::TypeDescriptor> {
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
	/// @param desc - TypeDescriptor to hash
	/// @return desc.hash
	size_t operator()(const arch::meta::rtti::TypeDescriptor& desc) const noexcept;
};

#include "TypeDesciptorHash.hpp"
