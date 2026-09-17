#pragma once

#include "ViewFwd.h"
//
#include "../CommonComponentPool.h"
#include "../Entity.h"
#include <archimedes/utils/Typelist.h>

namespace arch::ecs {

/// @brief General view iterator
template<class... Includes, class... Excludes>
requires(sizeof...(Includes) != 0 || sizeof...(Excludes) != 0)
class ViewIterator<Typelist<Includes...>, Typelist<Excludes...>> {
public:
	/// @brief Makes iterator bidirectional
	using iterator_concept = std::bidirectional_iterator_tag;
	/// @brief Makes iterator bidirectional
	using iterator_category = std::bidirectional_iterator_tag;
	/// @brief Iterator's value type
	using value_type = Entity;
	/// @brief Iterator's pointer type
	using pointer = const Entity*;
	/// @brief Iterator's reference type
	using reference = const Entity&;
	/// @brief Iterator's difference type
	using difference_type = std::ptrdiff_t;

	/// @brief Typelist with included types
	static constexpr auto includes = typelist<Includes...>;
	/// @brief Typelist with excluded types
	static constexpr auto excludes = typelist<Excludes...>;

	/// @brief Default constructor
	ViewIterator() noexcept = default;
	/// @brief Copy constructor
	ViewIterator(const ViewIterator&) noexcept = default;
	/// @brief Move constructor
	ViewIterator(ViewIterator&&) noexcept = default;

	/// @brief Copy-assignment operator
	ViewIterator& operator=(const ViewIterator&) noexcept = default;
	/// @brief Move-assignment operator
	ViewIterator& operator=(ViewIterator&&) noexcept = default;

	/// @brief View-based constructor (general)
	ViewIterator(const View<Typelist<Includes...>, Typelist<Excludes...>>& view, bool end) noexcept
		requires(includes.size() != 0);
	/// @brief View-based constructor (exclude-only)
	ViewIterator(const View<Typelist<Includes...>, Typelist<Excludes...>>& view, bool end) noexcept
		requires(includes.size() == 0);

	/// @brief Comparision operator
	bool operator==(const ViewIterator& other) const noexcept;
	/// @brief Comparision operator
	std::strong_ordering operator<=>(const ViewIterator& other) const noexcept;

	/// @brief Dereference operator
	const Entity& operator*() const noexcept;
	/// @brief Access operator
	const Entity* operator->() const noexcept;

	/// @brief Preincrement operator (include-only)
	ViewIterator& operator++() noexcept requires(includes.size() > 1 && excludes.size() == 0);
	/// @brief Preincrement operator (single include-only)
	ViewIterator& operator++() noexcept requires(includes.size() == 1 && excludes.size() == 0);
	/// @brief Preincrement operator (general)
	ViewIterator& operator++() noexcept requires(includes.size() != 0 && excludes.size() != 0);
	/// @brief Preincrement operator (exclude-only)
	ViewIterator& operator++() noexcept requires(includes.size() == 0 && excludes.size() != 0);

	/// @brief Postincrement operator
	ViewIterator operator++(int) noexcept;

	/// @brief Predecrement operator (include-only)
	ViewIterator& operator--() noexcept requires(includes.size() > 1 && excludes.size() == 0);
	/// @brief Predecrement operator (single include-only)
	ViewIterator& operator--() noexcept requires(includes.size() == 1 && excludes.size() == 0);
	/// @brief Predecrement operator (general)
	ViewIterator& operator--() noexcept requires(includes.size() != 0 && excludes.size() != 0);
	/// @brief Predecrement operator (exclude-only)
	ViewIterator& operator--() noexcept requires(includes.size() == 0 && excludes.size() != 0);

	/// @brief Postdecrement operator
	ViewIterator operator--(int) noexcept;

private:

	using Traits = ecs::_details::EntityTraits::Version;

	std::array<_details::CommonComponentPool*, includes.size()>::const_iterator _begin{}, _min{}, _end{};
	std::array<const _details::CommonComponentPool*, excludes.size()>::const_iterator _exclBegin{}, _exclEnd{};

	// Pointers to _min._dense
	const Entity *_denseBegin{}, *_denseI{}, *_denseEnd{};
};

} // namespace arch::ecs

#include "GeneralViewIterator.hpp"
