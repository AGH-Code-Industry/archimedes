#pragma once

#include <ranges>

namespace arch {

template<class It>
class BoundedRange: public std::ranges::view_interface<BoundedRange<It>> {
public:

	BoundedRange() noexcept = default;
	BoundedRange(const BoundedRange&) noexcept = default;
	BoundedRange(BoundedRange&&) noexcept = default;
	BoundedRange(It begin, It end) noexcept;

	BoundedRange& operator=(const BoundedRange&) noexcept = default;
	BoundedRange& operator=(BoundedRange&&) noexcept = default;

	It begin() const noexcept;
	It end() const noexcept;

private:
	It _begin, _end;
};

} // namespace arch

#include "BoundedRange.hpp"
