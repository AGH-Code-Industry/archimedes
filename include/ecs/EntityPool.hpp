#include "EntityPool.h"

namespace arch::ecs {

EntityPool::Iterator EntityPool::begin() noexcept {
	return _dense.begin();
}

EntityPool::ConstIterator EntityPool::begin() const noexcept {
	return _dense.begin();
}

EntityPool::ConstIterator EntityPool::cbegin() const noexcept {
	return _dense.cbegin();
}

EntityPool::Iterator EntityPool::end() noexcept {
	return _dense.begin() + _size;
}

EntityPool::ConstIterator EntityPool::end() const noexcept {
	return _dense.begin() + _size;
}

EntityPool::ConstIterator EntityPool::cend() const noexcept {
	return _dense.cbegin() + _size;
}

EntityPool::ReverseIterator EntityPool::rbegin() noexcept {
	return std::make_reverse_iterator(end());
}

EntityPool::ConstReverseIterator EntityPool::rbegin() const noexcept {
	return std::make_reverse_iterator(end());
}

EntityPool::ConstReverseIterator EntityPool::crbegin() const noexcept {
	return std::make_reverse_iterator(cend());
}

EntityPool::ReverseIterator EntityPool::rend() noexcept {
	return std::make_reverse_iterator(begin());
}

EntityPool::ConstReverseIterator EntityPool::rend() const noexcept {
	return std::make_reverse_iterator(begin());
}

EntityPool::ConstReverseIterator EntityPool::crend() const noexcept {
	return std::make_reverse_iterator(cbegin());
}

void EntityPool::swap(EntityPool& other) noexcept {
	std::swap(_sparse, other._sparse);
	std::swap(_dense, other._dense);
	std::swap(_size, other._size);
}

} // namespace arch::ecs
