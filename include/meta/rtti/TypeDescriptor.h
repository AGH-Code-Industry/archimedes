#pragma once

#include <compare>
#include <string>

namespace arch::meta::rtti {
/// @brief Class containing type data
/// @details There exists only one TypeDescriptor per type
class TypeDescriptor {
public:
	/// @brief Deleted copy constructor.
	TypeDescriptor(const TypeDescriptor&) = delete;
	/// @brief Deleted move constructor.
	TypeDescriptor(TypeDescriptor&&) = delete;

	/// @brief Deleted copy-assignment constructor.
	TypeDescriptor& operator=(const TypeDescriptor&) = delete;
	/// @brief Deleted move-assignment constructor.
	TypeDescriptor& operator=(TypeDescriptor&&) = delete;

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
} // namespace arch::meta::rtti
