#pragma once

#include <hier/ChildrenIterator.h>

namespace arch::scene {

class Entity;

class ChildrenIterator {
public:

	/// @brief Default constructor
	ChildrenIterator() noexcept = default;
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
	Entity operator*() const noexcept;

	/// @brief Prefix increment operator
	ChildrenIterator& operator++() noexcept;
	/// @brief Postfix increment operator
	ChildrenIterator operator++(int) noexcept;

	/// @brief Prefix decrement operator
	ChildrenIterator& operator--() noexcept;
	/// @brief Postfix decrement operator
	ChildrenIterator operator--(int) noexcept;

private:

	friend class Entity;

	ChildrenIterator(hier::ChildrenIterator base, Entity* parent) noexcept;

	hier::ChildrenIterator _current;
	Entity* _parent = nullptr;
};

} // namespace arch::scene

template<>
struct std::iterator_traits<arch::scene::ChildrenIterator> {
	using difference_type = std::ptrdiff_t;
	using value_type = arch::scene::Entity;
	using pointer = arch::scene::Entity*;
	using reference = arch::scene::Entity&;
	using iterator_category = std::bidirectional_iterator_tag;
};

