#include "ComponentPool.h"
#include "ComponentPoolIterator.h"

#define TEMPLATE_CE template<class C, class E>
#define ITER_CE ComponentPoolIterator<C, E>

namespace arch::ecs::_details {

TEMPLATE_CE
void ITER_CE::_update() noexcept {
	if (_valid()) { // update internal std::pair
		if constexpr (Traits::flag) {
			new (_value) ValueType((*_dense)[_i], true);
		} else {
			new (_value) ValueType((*_dense)[_i], *const_cast<C*>(*_componentPage + _offset));
		}
	}
}

TEMPLATE_CE
ITER_CE::ComponentPoolIterator(ComponentPool<C, E>* pool, size_t i) noexcept:
	_componentPage{ [&]() {
		if constexpr (Traits::flag) {
			return nullptr;
		} else {
			return pool->_components.data() + qdiv<Traits::pageSize>(i);
		}
	}() },
	_offset{ [&]() {
		if constexpr (Traits::flag) {
			return 0;
		} else {
			return qmod<Traits::pageSize>(i);
		}
	}() },
	_dense{ &pool->_dense },
	_i{ i } {
	_update();
}

TEMPLATE_CE
ITER_CE::Reference ITER_CE::_pair() const noexcept {
	return *reinterpret_cast<Pointer>(&const_cast<ITER_CE*>(this)->_value);
}

TEMPLATE_CE void ITER_CE::swap(ITER_CE& other) noexcept {
	std::swap(this->_componentPage, other._componentPage);
	std::swap(this->_offset, other._offset);
	std::swap(this->_dense, other._dense);
	std::swap(this->_i, other._i);
	std::swap(this->_pair(), other._pair());
}

TEMPLATE_CE
bool ITER_CE::_valid() const noexcept {
	return _i < _dense->size() and not ETraits::Version::hasNull((*_dense)[_i]);
}

TEMPLATE_CE
ITER_CE& ITER_CE::operator++() noexcept {
	if constexpr (Traits::inPlace) { // need to search for next valid
		do {
			if constexpr (not Traits::flag) {
				_offset = qmod<Traits::pageSize>(_offset + 1);
				if (not _offset) {
					++_componentPage;
				}
			}
		} while (++_i < _dense->size() and ETraits::Version::hasNull((*_dense)[_i]));
	} else {
		++_i;
		if constexpr (not Traits::flag) {
			_offset = qmod<Traits::pageSize>(_offset + 1);
			if (not _offset) {
				++_componentPage;
			}
		}
	}

	_update();
	return *this;
}

TEMPLATE_CE
ITER_CE ITER_CE::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

TEMPLATE_CE
ITER_CE& ITER_CE::operator--() noexcept {
	if constexpr (Traits::inPlace) { // need to search for next valid
		if (_i != 0 and _i != (size_t)-1) {
			do {
				if constexpr (not Traits::flag) {
					if (not _offset) {
						--_componentPage;
						_offset = Traits::pageSize - 1;
					} else {
						--_offset;
					}
				}
			} while (--_i != 0 and ETraits::Version::hasNull((*_dense)[_i]));
		} else {
			_i = (size_t)-1;
		}
	} else {
		if constexpr (not Traits::flag) {
			if (not _offset) {
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

TEMPLATE_CE
ITER_CE ITER_CE::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

TEMPLATE_CE
ITER_CE::Reference ITER_CE::operator*() const noexcept {
	return _pair();
}

TEMPLATE_CE
ITER_CE::Pointer ITER_CE::operator->() const noexcept {
	return &(_pair());
}

TEMPLATE_CE
bool ITER_CE::operator==(const ITER_CE& other) const noexcept {
	return _i == other._i;
}

TEMPLATE_CE
std::strong_ordering ITER_CE::operator<=>(const ITER_CE& other) const noexcept {
	return _i <=> other._i;
}

} // namespace arch::ecs::_details

#undef TEMPLATE_CE
#undef ITER_CE
