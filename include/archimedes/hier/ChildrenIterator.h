#pragma once

#include "ChildNode.h"
#include <ecs/Domain.h>

namespace arch::hier {

/// @brief Simple hierarchy iterator, providing access to children of an entity
/// @details Models std::bidirectional_iterator
class ChildrenIterator {
public:

	/// @brief Default constructor
	ChildrenIterator() noexcept = default;
	/// @brief Node constructor
	/// @param node - node to point to
	ChildrenIterator(const ChildNode* node) noexcept;
	/// @brief Copy constructor
	ChildrenIterator(const ChildrenIterator&) noexcept = default;
	/// @brief Move constructor
	ChildrenIterator(ChildrenIterator&&) noexcept = default;

	/// @brief Copy-assignment operator
	ChildrenIterator& operator=(const ChildrenIterator&) noexcept = default;
	/// @brief Move-assignment operator
	ChildrenIterator& operator=(ChildrenIterator&&) noexcept = default;

	/// @brief Equality operator
	bool operator==(const ChildrenIterator&) const noexcept = default;

	/// @brief Dereference operator
	ecs::Entity operator*() const noexcept;

	/// @brief Prefix increment operator
	ChildrenIterator& operator++() noexcept;
	/// @brief Postfix increment operator
	ChildrenIterator operator++(int) noexcept;

	/// @brief Prefix decrement operator
	ChildrenIterator& operator--() noexcept;
	/// @brief Postfix decrement operator
	ChildrenIterator operator--(int) noexcept;

private:

	const ChildNode* _current;
};

} // namespace arch::hier

template<>
struct std::iterator_traits<arch::hier::ChildrenIterator> {
	using difference_type = std::ptrdiff_t;
	using value_type = arch::ecs::Entity;
	using pointer = void;
	using reference = void;
	using iterator_category = std::bidirectional_iterator_tag;
};
