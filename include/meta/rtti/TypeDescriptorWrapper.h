#pragma once

#include <compare>

#include "TypeDescriptor.h"

namespace arch {
namespace meta {
namespace rtti {

class TypeDescriptor;

/// @brief Wrapper class for TypeDescriptor
class TypeDescriptorWrapper {
public:

	/// @brief Descriptor constructor
	/// @param desc - TypeDescriptor to set
	TypeDescriptorWrapper(const TypeDescriptor& desc) noexcept;
	/// @brief Default copy constructor
	TypeDescriptorWrapper(const TypeDescriptorWrapper&) noexcept = default;
	/// @brief Default move constructor
	TypeDescriptorWrapper(TypeDescriptorWrapper&&) noexcept = default;

	/// @brief Descriptor assignment operator
	/// @param desc - TypeDescriptor to set
	TypeDescriptorWrapper& operator=(const TypeDescriptor& desc) noexcept;
	/// @brief Default copy-assignment operator
	TypeDescriptorWrapper& operator=(const TypeDescriptorWrapper&) noexcept = default;
	/// @brief Default move-assignment operator
	TypeDescriptorWrapper& operator=(TypeDescriptorWrapper&&) noexcept = default;

	/// @brief Equality operator
	/// @param other - wrapper to compare
	bool operator==(const TypeDescriptorWrapper& other) const noexcept;
	/// @brief Comparision operator
	/// @param other - wrapper to compare
	std::strong_ordering operator<=>(const TypeDescriptorWrapper& other) const noexcept;

	/// @brief Returns held TypeDescriptor
	const TypeDescriptor& get() const noexcept;
	/// @brief Returns held TypeDescriptor
	const TypeDescriptor& operator*() const noexcept;

	/// @brief Returns name from TypeDescriptor
	std::string_view name() const noexcept;
	/// @brief Returns size from TypeDescriptor
	size_t size() const noexcept;
	/// @brief Returns alignment from TypeDescriptor
	size_t alignment() const noexcept;
	/// @brief Returns hash from TypeDescriptor
	size_t hash() const noexcept;

private:

	const TypeDescriptor* _desc;
};
} // namespace rtti

using rtti::TypeDescriptorWrapper;
} // namespace meta

using meta::TypeDescriptorWrapper;
} // namespace arch
