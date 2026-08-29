#include "SingleContinuousViewIterator.h"
//
#include "GeneralView.h"

#define TEMPLATE_I          \
	template<class Include> \
	requires(!_details::ComponentTraits<Include>::inPlace)
#define ITER_I ViewIterator<Typelist<Include>, Typelist<>>

namespace arch::ecs {

TEMPLATE_I
ITER_I::ViewIterator(const View<Typelist<Include>, Typelist<>>& view, bool end) noexcept {
	if (view._minCpoolIdx != (u32)-1) {
		if (end) {
			_denseI = std::to_address(view._cpools[0]->_dense.end());
		} else {
			_denseI = std::to_address(view._cpools[0]->_dense.begin());
		}
	}
}

TEMPLATE_I
bool ITER_I::operator==(const ViewIterator& other) const noexcept {
	return this->_denseI == other._denseI;
}

TEMPLATE_I
std::strong_ordering ITER_I::operator<=>(const ViewIterator& other) const noexcept {
	return this->_denseI <=> other._denseI;
}

TEMPLATE_I
const Entity& ITER_I::operator*() const noexcept {
	return *_denseI;
}

TEMPLATE_I
const Entity& ITER_I::operator[](std::ptrdiff_t n) const noexcept {
	return *(_denseI + n);
}

TEMPLATE_I
const Entity* ITER_I::operator->() const noexcept {
	return _denseI;
}

TEMPLATE_I
ITER_I& ITER_I::operator++() noexcept {
	++_denseI;
	return *this;
}

TEMPLATE_I
ITER_I ITER_I::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

TEMPLATE_I
ITER_I& ITER_I::operator--() noexcept {
	--_denseI;
	return *this;
}

TEMPLATE_I
ITER_I ITER_I::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

TEMPLATE_I
ITER_I& ITER_I::operator+=(std::ptrdiff_t n) noexcept {
	_denseI += n;
	return *this;
}

TEMPLATE_I
ITER_I ITER_I::operator+(std::ptrdiff_t n) const noexcept {
	auto temp = *this;
	temp += n;
	return temp;
}

TEMPLATE_I
ITER_I& ITER_I::operator-=(std::ptrdiff_t n) noexcept {
	_denseI -= n;
	return *this;
}

TEMPLATE_I
ITER_I ITER_I::operator-(std::ptrdiff_t n) const noexcept {
	auto temp = *this;
	temp -= n;
	return temp;
}

TEMPLATE_I
std::ptrdiff_t ITER_I::operator-(const ITER_I& other) const noexcept {
	return this->_denseI - other._denseI;
}

TEMPLATE_I
ITER_I operator+(std::ptrdiff_t n, const ITER_I& i) noexcept {
	return i + n;
}

} // namespace arch::ecs

#undef TEMPLATE_I
#undef ITER_I
