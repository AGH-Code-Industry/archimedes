#include <hier/Children.h>

namespace arch::hier {

Children::Iterator Children::begin() const noexcept {
	return ChildrenIterator(count ? first : &endSentinel);
}

Children::Iterator Children::cbegin() const noexcept {
	return begin();
}

Children::Iterator Children::end() const noexcept {
	return ChildrenIterator(&endSentinel);
}

Children::Iterator Children::cend() const noexcept {
	return end();
}

Children::ReverseIterator Children::rbegin() const noexcept {
	return std::make_reverse_iterator(end());
}

Children::ReverseIterator Children::crbegin() const noexcept {
	return std::make_reverse_iterator(end());
}

Children::ReverseIterator Children::rend() const noexcept {
	return std::make_reverse_iterator(begin());
}

Children::ReverseIterator Children::crend() const noexcept {
	return std::make_reverse_iterator(begin());
}

} // namespace arch::hier
