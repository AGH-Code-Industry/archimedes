#include "ViewIterator.h"
//
#include "View.h"

#define TEMPLATE_IE template<class... Includes, class... Excludes>
#define ITER_IE ViewIterator<TypeList<Includes...>, TypeList<Excludes...>>

// using macros instead of method calls decreased iteration time by 6-12%

// skips to the next valid entity
#define SKIP(op, to)                                                                                        \
	{                                                                                                       \
		const auto middleNext = _middle + 1;                                                                \
		if constexpr (sizeof...(Excludes) == 0) {                                                           \
			/* no excludes, skip check */                                                                   \
			while (_denseI != to &&                                                                         \
				   !(arch::ecs::_details::EntityTraits::Version::hasNotNull(*_denseI) &&                    \
					 std::all_of(                                                                           \
						 _begin,                                                                            \
						 _middle,                                                                           \
						 [entity = *_denseI](const auto cpool) noexcept { return cpool->contains(entity); } \
					 ) &&                                                                                   \
					 std::all_of(middleNext, _end, [entity = *_denseI](const auto cpool) noexcept {         \
						 return cpool->contains(entity);                                                    \
					 }))) {                                                                                 \
				op _denseI;                                                                                 \
			}                                                                                               \
		} else {                                                                                            \
			while (_denseI != to &&                                                                         \
				   !(arch::ecs::_details::EntityTraits::Version::hasNotNull(*_denseI) &&                    \
					 std::all_of(                                                                           \
						 _begin,                                                                            \
						 _middle,                                                                           \
						 [entity = *_denseI](const auto cpool) noexcept { return cpool->contains(entity); } \
					 ) &&                                                                                   \
					 std::all_of(                                                                           \
						 middleNext,                                                                        \
						 _end,                                                                              \
						 [entity = *_denseI](const auto cpool) noexcept { return cpool->contains(entity); } \
					 ) &&                                                                                   \
					 std::none_of(_exBegin, _exEnd, [entity = *_denseI](const auto cpool) noexcept {        \
						 return cpool && cpool->contains(entity);                                           \
					 }))) {                                                                                 \
				op _denseI;                                                                                 \
			}                                                                                               \
		}                                                                                                   \
	}

#define SKIP_NULLS_AND_OTHERS_FWD SKIP(++, _denseEnd)
#define SKIP_NULLS_AND_OTHERS_BWD SKIP(--, _denseBegin)

// skip nulls for in-place components
#define SKIP_NULLS(op, to)                                                                           \
	{                                                                                                \
		const auto middleNext = _middle + 1;                                                         \
		if constexpr (sizeof...(Excludes) == 0) {                                                    \
			/* no excludes, skip check */                                                            \
			while (_denseI != to && arch::ecs::_details::EntityTraits::Version::hasNull(*_denseI)) { \
				op _denseI;                                                                          \
			}                                                                                        \
		} else {                                                                                     \
			while (_denseI != to &&                                                                  \
				   (arch::ecs::_details::EntityTraits::Version::hasNull(*_denseI) ||                 \
					std::any_of(_exBegin, _exEnd, [entity = *_denseI](const auto cpool) noexcept {   \
						return cpool && cpool->contains(entity);                                     \
					}))) {                                                                           \
				op _denseI;                                                                          \
			}                                                                                        \
		}                                                                                            \
	}

#define SKIP_NULLS_FWD SKIP_NULLS(++, _denseEnd)
#define SKIP_NULLS_BWD SKIP_NULLS(--, _denseBegin)

// general forward skip
#define SKIP_FWD                                                                                     \
	if constexpr (includeCount != 1) {                                                               \
		SKIP_NULLS_AND_OTHERS_FWD;                                                                   \
	} else if constexpr (includeCount == 1 &&                                                        \
						 _details::ComponentTraits<                                                  \
							 std::remove_const_t<typename TypeList<Includes...>::front>>::inPlace) { \
		SKIP_NULLS_FWD;                                                                              \
	}
// general backward skip
#define SKIP_BWD                                                                                     \
	if constexpr (includeCount != 1) {                                                               \
		SKIP_NULLS_AND_OTHERS_BWD;                                                                   \
	} else if constexpr (includeCount == 1 &&                                                        \
						 _details::ComponentTraits<                                                  \
							 std::remove_const_t<typename TypeList<Includes...>::front>>::inPlace) { \
		SKIP_NULLS_BWD;                                                                              \
	}

namespace arch::ecs {

TEMPLATE_IE
ITER_IE::ViewIterator(
	const View<TypeList<Includes...>, TypeList<Excludes...>>* view,
	const size_t i,
	const bool valid
) noexcept:
	// if invalid, does not access any iterator
	// makes them identical, to skip any iteration attempt
	_begin{ view->_cpools.cbegin() },
	_middle{ valid ? _begin + view->_minIdx : _begin },
	_end{ valid ? view->_cpools.cend() : _begin },
	_denseBegin{ valid ? (*_middle)->_dense.data() : nullptr },
	_denseI{ valid ? _denseBegin + i : nullptr },
	_denseEnd{ valid ? _denseBegin + (*_middle)->_dense.size() : nullptr },
	_exBegin{ view->_exCpools.cbegin() },
	_exEnd{ view->_exCpools.cend() } {}

TEMPLATE_IE
ITER_IE& ITER_IE::operator++() noexcept {
	++_denseI;
	SKIP_FWD;
	return *this;
}

TEMPLATE_IE
ITER_IE ITER_IE::operator++(int) noexcept {
	auto temp = *this;
	++(*this);
	return temp;
}

TEMPLATE_IE
ITER_IE& ITER_IE::operator--() noexcept {
	--_denseI;
	SKIP_BWD;
	return *this;
}

TEMPLATE_IE
ITER_IE ITER_IE::operator--(int) noexcept {
	auto temp = *this;
	--(*this);
	return temp;
}

TEMPLATE_IE
Entity ITER_IE::operator*() const noexcept {
	return *_denseI;
}

TEMPLATE_IE
const Entity* ITER_IE::operator->() const noexcept {
	return _denseI;
}

TEMPLATE_IE
bool ITER_IE::operator==(const ViewIterator& other) const noexcept {
	return _denseI == other._denseI;
}

TEMPLATE_IE
auto ITER_IE::operator<=>(const ViewIterator& other) const noexcept {
	return _denseI <=> other._denseI;
}

} // namespace arch::ecs

#undef SKIP_BWD
#undef SKIP_FWD
#undef SKIP_NULLS_BWD
#undef SKIP_NULLS_FWD
#undef SKIP_NULLS
#undef SKIP_NULLS_AND_OTHERS_FWD
#undef SKIP_NULLS_AND_OTHERS_BWD
#undef SKIP
#undef ITER_IE
#undef TEMPLATE_IE
