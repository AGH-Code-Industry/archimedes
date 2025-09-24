#include "ExcludingViewIterator.h"
//
#include "ExcludingView.h"

#define TEMPLATE_E template<class... Excludes>
#define ITER_E ViewIterator<TypeList<>, TypeList<Excludes...>>

namespace arch::ecs {

TEMPLATE_E
ITER_E::ViewIterator(const View<TypeList<>, TypeList<Excludes...>>* view, const size_t i) noexcept:
	_denseBegin{ std::to_address(view->_pool->_dense.cbegin()) },
	_denseI{ _denseBegin + i },
	_denseEnd{ std::to_address(view->_pool->_dense.cend()) },
	_exBegin{ view->_excludedCpools.cbegin() },
	_exEnd{ view->_excludedCpools.cend() } {
	if constexpr (excludeCount != 0) {
		if (i == 0) {
			while (_denseI != _denseEnd &&
				   std::any_of(_exBegin, _exEnd, [entity = *_denseI](const auto cpool) noexcept {
					   return cpool && cpool->contains(entity);
				   })) {
				++_denseI;
			}
		}
	}
}

TEMPLATE_E
ITER_E& ITER_E::operator++() noexcept {
	++_denseI;
	if constexpr (excludeCount != 0) {
		while (_denseI != _denseEnd && std::any_of(_exBegin, _exEnd, [entity = *_denseI](const auto cpool) noexcept {
				   return cpool && cpool->contains(entity);
			   })) {
			++_denseI;
		}
	}
	return *this;
}

TEMPLATE_E
ITER_E ITER_E::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

TEMPLATE_E
ITER_E& ITER_E::operator--() noexcept {
	--_denseI;
	if constexpr (excludeCount != 0) {
		while (_denseI != _denseBegin && std::any_of(_exBegin, _exEnd, [entity = *_denseI](const auto cpool) noexcept {
				   return cpool && cpool->contains(entity);
			   })) {
			--_denseI;
		}
	}
	return *this;
}

TEMPLATE_E
ITER_E ITER_E::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

TEMPLATE_E
Entity ITER_E::operator*() const noexcept {
	return *_denseI;
}

TEMPLATE_E
const Entity* ITER_E::operator->() const noexcept {
	return _denseI;
}

TEMPLATE_E
bool ITER_E::operator==(const ViewIterator& other) const noexcept {
	return _denseI == other._denseI;
}

TEMPLATE_E
auto ITER_E::operator<=>(const ViewIterator& other) const noexcept {
	return _denseI <=> other._denseI;
}

} // namespace arch::ecs

#undef TEMPLATE_E
#undef ITER_E
