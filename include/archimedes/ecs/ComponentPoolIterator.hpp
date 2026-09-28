#include "ComponentPool.h"
#include "ComponentPoolIterator.h"

#define TEMPLATE_C template<class C>
#define ITER_C ComponentPoolIterator<C>

namespace arch::ecs::_details {

TEMPLATE_C
ITER_C::ComponentPoolIterator(ComponentPool<C>* pool, size_t i) noexcept:
	_componentPage{ [&]() {
		if constexpr (Traits::flag) {
			return nullptr;
		} else {
			return pool->_components.data() + i / Traits::pageSize;
		}
	}() },
	_offset{ [&]() {
		if constexpr (Traits::flag) {
			return 0;
		} else {
			return i % Traits::pageSize;
		}
	}() },
	_dense{ &pool->_dense },
	_i{ i } {}

TEMPLATE_C void ITER_C::swap(ITER_C& other) noexcept {
	std::swap(this->_componentPage, other._componentPage);
	std::swap(this->_offset, other._offset);
	std::swap(this->_dense, other._dense);
	std::swap(this->_i, other._i);
}

TEMPLATE_C
ITER_C& ITER_C::operator++() noexcept requires(Traits::inPlace)
{
	do {
		if constexpr (!Traits::flag) {
			_offset = (_offset + 1) % Traits::pageSize;
			if (!_offset) {
				++_componentPage;
			}
		}
	} while (++_i < _dense->size() && ETraits::Version::hasNull((*_dense)[_i]));

	return *this;
}

TEMPLATE_C
ITER_C& ITER_C::operator++() noexcept requires(!Traits::inPlace)
{
	++_i;
	if constexpr (!Traits::flag) {
		_offset = (_offset + 1) % Traits::pageSize;
		if (!_offset) {
			++_componentPage;
		}
	}

	return *this;
}

TEMPLATE_C
ITER_C ITER_C::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

TEMPLATE_C
ITER_C& ITER_C::operator--() noexcept requires(Traits::inPlace)
{
	if (_i != 0 && _i != (size_t)-1) {
		do {
			if constexpr (!Traits::flag) {
				if (!_offset) {
					--_componentPage;
					_offset = Traits::pageSize - 1;
				} else {
					--_offset;
				}
			}
		} while (--_i != 0 && ETraits::Version::hasNull((*_dense)[_i]));
	} else {
		_i = (size_t)-1;
	}

	return *this;
}

TEMPLATE_C
ITER_C& ITER_C::operator--() noexcept requires(!Traits::inPlace)
{
	if constexpr (!Traits::flag) {
		if (!_offset) {
			--_componentPage;
			_offset = Traits::pageSize - 1;
		} else {
			--_offset;
		}
	}
	--_i;

	return *this;
}

TEMPLATE_C
ITER_C ITER_C::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

TEMPLATE_C
ITER_C::Reference ITER_C::operator*() const noexcept {
	return *const_cast<C*>(*_componentPage + _offset);
}

TEMPLATE_C
const Entity& ITER_C::entity() const noexcept {
	return (*_dense)[_i];
}

TEMPLATE_C ITER_C::Pointer ITER_C::operator->() const noexcept {
	return const_cast<C*>(*_componentPage + _offset);
}

TEMPLATE_C
ITER_C::Reference ITER_C::operator[](std::ptrdiff_t n) const noexcept requires(!Traits::inPlace)
{
	return *(*this + n);
}

TEMPLATE_C
ITER_C& ITER_C::operator+=(std::ptrdiff_t n) noexcept requires(!Traits::inPlace)
{
	_i += n;
	if constexpr (!Traits::flag) {
		auto newOffset = (std::ptrdiff_t)_offset + n;
		_componentPage += newOffset / Traits::pageSize;
		_offset = newOffset % Traits::pageSize;
	}

	return *this;
}

TEMPLATE_C
ITER_C ITER_C::operator+(std::ptrdiff_t n) const noexcept requires(!Traits::inPlace)
{
	auto temp = *this;
	temp += n;
	return temp;
}

TEMPLATE_C
ITER_C& ITER_C::operator-=(std::ptrdiff_t n) noexcept requires(!Traits::inPlace)
{
	_i -= n;
	if constexpr (!Traits::flag) {
		auto newOffset = (std::ptrdiff_t)_offset - n;
		if (newOffset < 0) {
			auto stepBack = (-newOffset + (Traits::pageSize - 1)) / Traits::pageSize;
			_componentPage -= stepBack;
			newOffset += stepBack * Traits::pageSize;
		}
		_componentPage += newOffset / Traits::pageSize;
		_offset = newOffset % Traits::pageSize;
	}
}

TEMPLATE_C
ITER_C ITER_C::operator-(std::ptrdiff_t n) const noexcept requires(!Traits::inPlace)
{
	auto temp = *this;
	temp -= n;
	return temp;
}

TEMPLATE_C
std::ptrdiff_t ITER_C::operator-(const ComponentPoolIterator& other) const noexcept requires(!Traits::inPlace)
{
	return (std::ptrdiff_t)_i - (std::ptrdiff_t)other._i;
}

TEMPLATE_C
bool ITER_C::operator==(const ITER_C& other) const noexcept {
	return _i == other._i;
}

TEMPLATE_C
std::strong_ordering ITER_C::operator<=>(const ITER_C& other) const noexcept {
	return _i <=> other._i;
}

TEMPLATE_C
requires(!_details::ComponentTraits<C>::inPlace) ITER_C operator+(std::ptrdiff_t n, const ITER_C& i) noexcept {
	return i + n;
}

} // namespace arch::ecs::_details

#undef TEMPLATE_C
#undef ITER_C
