#include "ComponentPool.h"
#include "ComponentPoolIterator.h"

#define TEMPLATE_C template<class C>
#define ITER_C ComponentPoolIterator<C>

namespace arch::ecs::_details {

TEMPLATE_C
void ITER_C::_update() noexcept {
	if (_valid()) { // update internal std::pair
		if constexpr (Traits::flag) {
			new (_value) ValueType((*_dense)[_i], true);
		} else {
			new (_value) ValueType((*_dense)[_i], *const_cast<C*>(*_componentPage + _offset));
		}
	}
}

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
	_i{ i } {
	_update();
}

TEMPLATE_C
ITER_C::Reference ITER_C::_pair() const noexcept {
	return *reinterpret_cast<Pointer>(&const_cast<ITER_C*>(this)->_value);
}

TEMPLATE_C void ITER_C::swap(ITER_C& other) noexcept {
	std::swap(this->_componentPage, other._componentPage);
	std::swap(this->_offset, other._offset);
	std::swap(this->_dense, other._dense);
	std::swap(this->_i, other._i);
	std::swap(this->_pair(), other._pair());
}

TEMPLATE_C
bool ITER_C::_valid() const noexcept {
	return _i < _dense->size() && !ETraits::Version::hasNull((*_dense)[_i]);
}

TEMPLATE_C
ITER_C& ITER_C::operator++() noexcept {
	if constexpr (Traits::inPlace) { // need to search for next valid
		do {
			if constexpr (!Traits::flag) {
				_offset = (_offset + 1) % Traits::pageSize;
				if (!_offset) {
					++_componentPage;
				}
			}
		} while (++_i < _dense->size() && ETraits::Version::hasNull((*_dense)[_i]));
	} else {
		++_i;
		if constexpr (!Traits::flag) {
			_offset = (_offset + 1) % Traits::pageSize;
			if (!_offset) {
				++_componentPage;
			}
		}
	}

	_update();
	return *this;
}

TEMPLATE_C
ITER_C ITER_C::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

TEMPLATE_C
ITER_C& ITER_C::operator--() noexcept {
	if constexpr (Traits::inPlace) { // need to search for next valid
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
	} else {
		if constexpr (!Traits::flag) {
			if (!_offset) {
				--_componentPage;
				_offset = Traits::pageSize - 1;
			} else {
				--_offset;
			}
		}
		--_i;
	}

	_update();
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
	return _pair();
}

TEMPLATE_C
ITER_C::Pointer ITER_C::operator->() const noexcept {
	return &(_pair());
}

TEMPLATE_C
bool ITER_C::operator==(const ITER_C& other) const noexcept {
	return _i == other._i;
}

TEMPLATE_C
std::strong_ordering ITER_C::operator<=>(const ITER_C& other) const noexcept {
	return _i <=> other._i;
}

} // namespace arch::ecs::_details

#undef TEMPLATE_C
#undef ITER_C
