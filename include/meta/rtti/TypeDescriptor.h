#pragma once

#include <compare>
#include <string>
#include <typeinfo>

namespace arch {
namespace meta {
namespace rtti {
class TypeDescriptorWrapper;

/// @brief Class containing type data
/// @brief It is guaranteed that only one TypeDescriptor exists per type
class TypeDescriptor {
public:

	/// @brief Standardized type name
	std::string name;
	/// @brief Size of type
	size_t size;
	/// @brief Alignment of type
	size_t alignment;
	/// @brief Standardized hash of name
	/// @details Computed using cyclic polynomial rolling hash algorithm
	size_t hash;

	/// @brief Equality operator
	/// @param other - TypeDescriptor to compare
	bool operator==(const TypeDescriptor& other) const noexcept;
	/// @brief Equality operator
	/// @param other - std::type_info to compare
	bool operator==(const std::type_info& other) const noexcept;
	/// @brief Comparision operator
	/// @param other - TypeDescriptor to compare
	std::strong_ordering operator<=>(const TypeDescriptor& other) const noexcept;

	/// @brief Wraps this TypeDescriptor
	/// @return TypeDescriptorWrapper(*this)
	TypeDescriptorWrapper wrap() const noexcept;

private:
	// used only by TypeDescriptorOwner
	TypeDescriptor() = default;

	template<class T>
	friend class TypeDescriptorOwner;

	const std::type_info* _typeid;
};
} // namespace rtti

using rtti::TypeDescriptor;
} // namespace meta

using meta::TypeDescriptor;
} // namespace arch
