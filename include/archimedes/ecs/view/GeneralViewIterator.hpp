#include "GeneralViewIterator.h"
//
#include "ExcludingView.h"
#include "GeneralView.h"

#define TEMPLATE_IE                                \
	template<class... Includes, class... Excludes> \
	requires(sizeof...(Includes) != 0 || sizeof...(Excludes) != 0)

#define ITER_IE ViewIterator<Typelist<Includes...>, Typelist<Excludes...>>

namespace arch::ecs {

TEMPLATE_IE
ITER_IE::ViewIterator(const View<Typelist<Includes...>, Typelist<Excludes...>>& view, bool end) noexcept
	requires(includes.size() != 0)
{
	if (view._minCpoolIdx != (u32)-1) {
		_min = view._cpools.begin() + view._minCpoolIdx;
		_begin = view._cpools.begin();
		_end = view._cpools.end();

		_denseBegin = std::to_address((*_min)->_dense.begin());
		_denseEnd = std::to_address((*_min)->_dense.end());

		if constexpr (excludes.size() != 0) {
			_exclBegin = view._cpoolsExcl.begin();
			_exclEnd = view._cpoolsExcl.end();
		}

		if (end) {
			// assume _denseEnd as the end
			// operator++ on the last element will advance until meets _denseEnd
			// therefore operator-- here is not required
			_denseI = _denseEnd;
		} else {
			// go past-the-first and advance
			// skips nulls, etc.
			_denseI = _denseBegin - 1;
			++(*this);
		}
	}
}

TEMPLATE_IE
ITER_IE::ViewIterator(const View<Typelist<Includes...>, Typelist<Excludes...>>& view, bool end) noexcept
	requires(includes.size() == 0)
{
	_exclBegin = view._cpoolsExcl.begin();
	_exclEnd = view._cpoolsExcl.end();

	_denseBegin = &*view._domain._entityPool.begin();
	_denseEnd = &*view._domain._entityPool.end();

	if (end) {
		// assume _denseEnd as the end
		// operator++ on the last element will advance until meets _denseEnd
		// therefore operator-- here is not required
		_denseI = _denseEnd;
	} else {
		// go past-the-first and advance
		// skips nulls, etc.
		_denseI = _denseBegin - 1;
		++(*this);
	}
}

TEMPLATE_IE
bool ITER_IE::operator==(const ViewIterator& other) const noexcept {
	return _denseI == other._denseI;
}

TEMPLATE_IE
std::strong_ordering ITER_IE::operator<=>(const ViewIterator& other) const noexcept {
	return _denseI <=> other._denseI;
}

TEMPLATE_IE
const Entity& ITER_IE::operator*() const noexcept {
	return *_denseI;
}

TEMPLATE_IE
const Entity* ITER_IE::operator->() const noexcept {
	return &*_denseI;
}

TEMPLATE_IE
ITER_IE& ITER_IE::operator++() noexcept requires(includes.size() > 1 && excludes.size() == 0)
{
	if (_denseI < _denseEnd) {
		++_denseI;
	}

	auto notContains = [&](const auto cpool) {
		return !cpool->contains(*_denseI);
	};

	const auto minNext = _min + 1;
	while (_denseI < _denseEnd &&
		   (Traits::hasNull(*_denseI) || std::any_of(_begin, _min, notContains) ||
			std::any_of(minNext, _end, notContains))) {
		// skip nulls and not contained
		++_denseI;
	}

	return *this;
}

TEMPLATE_IE
ITER_IE& ITER_IE::operator++() noexcept requires(includes.size() == 1 && excludes.size() == 0)
{
	if (_denseI < _denseEnd) {
		++_denseI;
	}

	while (_denseI < _denseEnd && Traits::hasNull(*_denseI)) {
		// skip nulls
		++_denseI;
	}

	return *this;
}

TEMPLATE_IE
ITER_IE& ITER_IE::operator++() noexcept requires(includes.size() != 0 && excludes.size() != 0)
{
	if (_denseI < _denseEnd) {
		++_denseI;
	}

	auto notContains = [&](const auto cpool) {
		return !cpool->contains(*_denseI);
	};
	auto exclContains = [&](const auto cpool) {
		return cpool && cpool->contains(*_denseI);
	};

	const auto minNext = _min + 1;

	while (_denseI < _denseEnd &&
		   (Traits::hasNull(*_denseI) || std::any_of(_begin, _min, notContains) ||
			std::any_of(minNext, _end, notContains) || std::any_of(_exclBegin, _exclEnd, exclContains))) {
		// skip nulls, not contained and excluded
		++_denseI;
	}

	return *this;
}

TEMPLATE_IE
ITER_IE& ITER_IE::operator++() noexcept requires(includes.size() == 0 && excludes.size() != 0)
{
	if (_denseI < _denseEnd) {
		++_denseI;
	}

	auto exclContains = [&](const auto cpool) {
		return cpool && cpool->contains(*_denseI);
	};

	while (_denseI < _denseEnd || std::any_of(_exclBegin, _exclEnd, exclContains)) {
		// skip excluded
		++_denseI;
	}
}

TEMPLATE_IE ITER_IE ITER_IE::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

TEMPLATE_IE
ITER_IE& ITER_IE::operator--() noexcept requires(includes.size() > 1 && excludes.size() == 0)
{
	if (_denseBegin <= _denseI) {
		--_denseI;
	}

	auto notContains = [&](const auto cpool) {
		return !cpool->contains(*_denseI);
	};

	const auto minNext = _min + 1;

	while (_denseBegin <= _denseI &&
		   (Traits::hasNull(*_denseI) || std::any_of(_begin, _min, notContains) ||
			std::any_of(minNext, _end, notContains))) {
		// skip nulls and not contained
		--_denseI;
	}

	return *this;
}

TEMPLATE_IE
ITER_IE& ITER_IE::operator--() noexcept requires(includes.size() == 1 && excludes.size() == 0)
{
	if (_denseBegin <= _denseI) {
		--_denseI;
	}

	while (_denseBegin <= _denseI && Traits::hasNull(*_denseI)) {
		// skip nulls
		--_denseI;
	}

	return *this;
}

TEMPLATE_IE
ITER_IE& ITER_IE::operator--() noexcept requires(includes.size() != 0 && excludes.size() != 0)
{
	if (_denseBegin <= _denseI) {
		--_denseI;
	}

	auto notContains = [&](const auto cpool) {
		return !cpool->contains(*_denseI);
	};
	auto exclContains = [&](const auto cpool) {
		return cpool && cpool->contains(*_denseI);
	};

	const auto minNext = _min + 1;

	while (_denseBegin <= _denseI &&
		   (Traits::hasNull(*_denseI) || std::any_of(_begin, _min, notContains) ||
			std::any_of(minNext, _end, notContains) || std::any_of(_exclBegin, _exclEnd, exclContains))) {
		// skip nulls, not conatined and excluded
		--_denseI;
	}

	return *this;
}

TEMPLATE_IE
ITER_IE& ITER_IE::operator--() noexcept requires(includes.size() == 0 && excludes.size() != 0)
{
	if (_denseBegin <= _denseI) {
		--_denseI;
	}

	auto exclContains = [&](const auto cpool) {
		return cpool && cpool->contains(*_denseI);
	};

	while (_denseBegin <= _denseI || std::any_of(_exclBegin, _exclEnd, exclContains)) {
		// skip excluded
		--_denseI;
	}
}

TEMPLATE_IE ITER_IE ITER_IE::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

} // namespace arch::ecs

#undef TEMPLATE_IE
#undef ITER_IE
