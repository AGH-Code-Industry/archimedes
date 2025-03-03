#pragma once

#include <iterator>

#include <hier/HierarchyIterator.h>

namespace arch::scene {

class Entity;

class HierarchyIterator {
public:

	/// @brief Default constructor
	HierarchyIterator() noexcept = default;
	/// @brief Copy constructor
	HierarchyIterator(const HierarchyIterator&) noexcept = default;
	/// @brief Move constructor
	HierarchyIterator(HierarchyIterator&&) noexcept = default;

	/// @brief Copy-assignment operator
	HierarchyIterator& operator=(const HierarchyIterator&) noexcept = default;
	/// @brief Move-assignment operator
	HierarchyIterator& operator=(HierarchyIterator&&) noexcept = default;

	/// @brief Equality operator
	bool operator==(const HierarchyIterator&) const noexcept = default;

	/// @brief Dereference operator
	Entity operator*() const noexcept;

	/// @brief Prefix increment operator
	HierarchyIterator& operator++() noexcept;
	/// @brief Postfix increment operator
	HierarchyIterator operator++(int) noexcept;

	/// @brief Prefix decrement operator
	HierarchyIterator& operator--() noexcept;
	/// @brief Postfix decrement operator
	HierarchyIterator operator--(int) noexcept;

private:

	friend class Entity;

	HierarchyIterator(hier::_details::HierarchyIterator base, Entity* parent) noexcept;

	hier::_details::HierarchyIterator _current;
	Entity* _parent = nullptr;
};

} // namespace arch::scene

template<>
struct std::iterator_traits<arch::scene::HierarchyIterator> {
	using difference_type = std::ptrdiff_t;
	using value_type = arch::scene::Entity;
	using pointer = void;
	using reference = void;
	using iterator_category = std::bidirectional_iterator_tag;
};
