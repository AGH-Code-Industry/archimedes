#include "ComponentPool.h"
#include "ComponentPoolIterator.h"

#define TEMPLATE template<class C, class E>
#define ITER ComponentPoolIterator<C, E>

namespace arch::ecs::_details {

TEMPLATE
ITER::ComponentPoolIterator(ComponentPool<C, E>* pool, size_t i) noexcept:
	_componentPage{ pool->_components.data() + qdiv<Traits::pageSize>(i) },
	_offset{ qmod<Traits::pageSize>(i) },
	_dense{ &pool->_dense },
	_i{ i } {
	if (_valid()) {
		new (_value) ValueType((*_dense)[_i], *const_cast<C*>(*_componentPage + _offset));
	}
}

TEMPLATE
ITER::Reference ITER::_pair() const noexcept {
	return *reinterpret_cast<Pointer>(&const_cast<ITER*>(this)->_value);
}

TEMPLATE void ITER::swap(ITER& other) noexcept {
	std::swap(this->_componentPage, other._componentPage);
	std::swap(this->_offset, other._offset);
	std::swap(this->_dense, other._dense);
	std::swap(this->_i, other._i);
	std::swap(this->_pair(), other._pair());
}

TEMPLATE
bool ITER::_valid() const noexcept {
	return _i < _dense->size() and not ETraits::Version::hasNull((*_dense)[_i]);
}

TEMPLATE
ITER& ITER::operator++() noexcept {
	if constexpr (Traits::inPlace) { // need to search for next valid
		do {
			_offset = qmod<Traits::pageSize>(_offset + 1);
			if (not _offset) {
				++_componentPage;
			}
		} while (++_i < _dense->size() and ETraits::Version::hasNull((*_dense)[_i]));
	} else {
		++_i;
		_offset = qmod<Traits::pageSize>(_offset + 1);
		if (not _offset) {
			++_componentPage;
		}
	}

	if (_valid()) {
		new (_value) ValueType((*_dense)[_i], *const_cast<C*>(*_componentPage + _offset));
	}
	return *this;
}

TEMPLATE
ITER ITER::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

TEMPLATE
ITER& ITER::operator--() noexcept {
	if constexpr (Traits::inPlace) { // need to search for next valid
		if (_i != 0 and _i != (size_t)-1) {
			do {
				if (not _offset) {
					--_componentPage;
					_offset = Traits::pageSize - 1;
				} else {
					--_offset;
				}
			} // --i: 0 -> (size_t)-1 PRZYPAŁ!!
			while (--_i != 0 and ETraits::Version::hasNull((*_dense)[_i]));
		} else {
			_i = (size_t)-1;
		}
	} else {
		if (not _offset) {
			--_componentPage;
			_offset = Traits::pageSize - 1;
		} else {
			--_offset;
		}
		--_i;
	}

	if (_valid()) {
		new (_value) ValueType((*_dense)[_i], *const_cast<C*>(*_componentPage + _offset));
	}
	return *this;
}

TEMPLATE
ITER ITER::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

TEMPLATE
ITER::Reference ITER::operator*() const noexcept {
	return _pair();
}

TEMPLATE
ITER::Pointer ITER::operator->() const noexcept {
	return &_pair();
}

TEMPLATE
bool ITER::operator==(const ITER& other) const noexcept {
	return _dense == other._dense and _i == other._i;
}

TEMPLATE
std::strong_ordering ITER::operator<=>(const ITER& other) const noexcept {
	return _i <=> other._i;
}

} // namespace arch::ecs::_details

#undef TEMPLATE
#undef ITER
