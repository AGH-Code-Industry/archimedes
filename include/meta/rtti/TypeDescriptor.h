#pragma once

#include <compare>
#include <iostream>
#include <string>

namespace arch {
namespace meta {
namespace rtti {
/// @brief Class containing type data
class TypeDescriptor {
public:
	/// @brief Copy constructor.
	TypeDescriptor(const TypeDescriptor&) noexcept = default;
	/// @brief Move constructor.
	TypeDescriptor(TypeDescriptor&&) noexcept = default;

	/// @brief Deleted copy-assignment operator.
	TypeDescriptor& operator=(const TypeDescriptor&) noexcept = default;
	/// @brief Deleted move-assignment operator.
	TypeDescriptor& operator=(TypeDescriptor&&) noexcept = default;

	/// @brief Standardized type name
	std::string name;
	/// @brief Size of type
	size_t size;
	/// @brief Alignment of type
	size_t alignment;
	/// @brief Standardized hash of name
	/// @details Computed using cyclic polynomial rolling hash algorithm
	size_t hash;

	/// @brief Equality operator.
	/// @param other - TypeDescriptor to compare
	bool operator==(const TypeDescriptor& other) const noexcept;
	/// @brief Comparision operator
	/// @brief Equal to this->name <=> other.name
	std::strong_ordering operator<=>(const TypeDescriptor& other) const noexcept;

private:
	// used only by TypeDescriptorOwner
	TypeDescriptor() = default;

	template<class T>
	friend class TypeDescriptorOwner;
};
} // namespace rtti

using rtti::TypeDescriptor;
} // namespace meta

using meta::TypeDescriptor;
} // namespace arch
