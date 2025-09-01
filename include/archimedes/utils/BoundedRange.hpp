#include "BoundedRange.h"

namespace arch {

template<class It>
BoundedRange<It>::BoundedRange(It begin, It end) noexcept: _begin{ std::move(begin) }, _end{ std::move(end) } {}

template<class It>
It BoundedRange<It>::begin() const noexcept {
	return _begin;
}

template<class It>
It BoundedRange<It>::end() const noexcept {
	return _end;
}

} // namespace arch
