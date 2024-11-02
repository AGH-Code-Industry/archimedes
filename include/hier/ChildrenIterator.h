#pragma once

#include "ChildNode.h"
#include <ecs/Domain.h>

namespace arch::hier {

/// @brief Simple hierarchy iterator, providing access to children of an entity
/// @details Models std::bidirectional_iterator
class ChildrenIterator {
public:

	ChildrenIterator(const ChildNode* node) noexcept;
	ChildrenIterator() noexcept = default;
	ChildrenIterator(const ChildrenIterator&) noexcept = default;
	ChildrenIterator(ChildrenIterator&&) noexcept = default;

	ChildrenIterator& operator=(const ChildrenIterator&) noexcept = default;
	ChildrenIterator& operator=(ChildrenIterator&&) noexcept = default;

	bool operator==(const ChildrenIterator&) const noexcept = default;

	ecs::Entity operator*() const noexcept;

	ChildrenIterator operator++(int) noexcept;
	ChildrenIterator& operator++() noexcept;

	ChildrenIterator operator--(int) noexcept;
	ChildrenIterator& operator--() noexcept;

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
