#pragma once

#include <archimedes/ecs/Entity.h>

namespace arch::hier {

class HierarchyNode;
class ChildNode;

namespace _details { // NOLINT

/// @brief Simple hierarchy iterator, providing recursive access to descendants of an entity
/// @details Models std::bidirectional_iterator
class HierarchyIterator {
public:

	/// @brief Default constructor
	HierarchyIterator() noexcept = default;
	/// @brief Node constructor
	/// @param node - node to point to
	HierarchyIterator(const ChildNode* node) noexcept;
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
	ecs::Entity operator*() const noexcept;

	/// @brief Prefix increment operator
	HierarchyIterator& operator++() noexcept;
	/// @brief Postfix increment operator
	HierarchyIterator operator++(int) noexcept;

	/// @brief Prefix decrement operator
	HierarchyIterator& operator--() noexcept;
	/// @brief Postfix decrement operator
	HierarchyIterator operator--(int) noexcept;

private:

	const ChildNode* _current;
};

} // namespace _details

} // namespace arch::hier

template<>
struct std::iterator_traits<arch::hier::_details::HierarchyIterator> {
	using difference_type = std::ptrdiff_t;
	using value_type = arch::ecs::Entity;
	using pointer = void;
	using reference = void;
	using iterator_category = std::bidirectional_iterator_tag;
};
