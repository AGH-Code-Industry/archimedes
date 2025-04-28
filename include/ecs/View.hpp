#include "View.h"
//
#include <ranges>

#include "Domain.h"
#include "IsEntity.h"
#include <tUtils/Functions/CallableTraits.h>
#include <tUtils/Functions/IsApplicable.h>

#define TEMPLATE_IE template<class... Includes, class... Excludes>
#define VIEW_IE View<TypeList<Includes...>, TypeList<Excludes...>>

namespace arch::ecs {

TEMPLATE_IE
VIEW_IE::View(Domain* domain) noexcept:
	_cpools{ (_details::CommonComponentPool*)domain->_tryGetCPool<Includes>()... },
	_exCpools{ (_details::CommonComponentPool*)domain->_tryGetCPool<Excludes>()... },
	_minIdx{ _getMinIdx() } {}

TEMPLATE_IE
u32 VIEW_IE::_getMinIdx() const noexcept {
	u32 result = 0;
	for (auto i = 0u; i != _cpools.size(); ++i) {
		if (!_cpools[i]) {
			return (u32)-1;
		}
		if (_cpools[i]->count() < _cpools[result]->count()) {
			result = i;
		}
	}
	return result;
}

TEMPLATE_IE
VIEW_IE::Iterator VIEW_IE::begin() const noexcept {
	return Iterator(this, 0, _minIdx != (u32)-1);
}

TEMPLATE_IE
VIEW_IE::Iterator VIEW_IE::cbegin() const noexcept {
	return begin();
}

TEMPLATE_IE
VIEW_IE::Iterator VIEW_IE::end() const noexcept {
	return Iterator(this, _minIdx == (u32)-1 ? 0 : _cpools[_minIdx]->_dense.size(), _minIdx != (u32)-1);
}

TEMPLATE_IE
VIEW_IE::Iterator VIEW_IE::cend() const noexcept {
	return end();
}

TEMPLATE_IE
template<class Fn>
void VIEW_IE::forEach(Fn&& fn) noexcept {
	using Traits = tUtils::CallableTraits<Fn>;

	if constexpr (Traits::isCallable) {
		static constexpr bool isFirstEntity = _details::IsEntity<typename Traits::Args::front>::value;

		using Args = Traits::Args;
		using ArgsNonEntity = std::conditional_t<isFirstEntity, typename Args::popFront, Args>;

		using WantedComponents = ArgsNonEntity::template transform<std::remove_reference>;
		using ConstGets = GetTL::template filter<std::is_const>::template transform<std::remove_const>;

		static_assert(!ConstGets::template containsAnyFrom<WantedComponents>, "Wanted components exceed this view");

		if (_minIdx == (u32)-1) {
			return;
		}

		_forEach<isFirstEntity>(std::forward<Fn>(fn), WantedComponents());
	} else if constexpr (tUtils::isApplicableV<
							 Fn,
							 typename GetTL::template transform<std::add_lvalue_reference>::toTuple>) {
		_forEach<false>(std::forward<Fn>(fn), GetTL());
	} else if constexpr (tUtils::isApplicableV<
							 Fn,
							 typename GetTL::template transform<std::add_lvalue_reference>::template prepend<
								 Entity>::toTuple>) {
		_forEach<true>(std::forward<Fn>(fn), GetTL());
	} else {
		static_assert(false, "Fn cannot be called on view");
	}
}

TEMPLATE_IE
template<bool PassEntity, class Fn, class... Cs>
void VIEW_IE::_forEach(Fn&& fn, TypeList<Cs...>) noexcept {
	using ComponentList = TypeList<Cs...>;
	using ActualComponents = ComponentList::template transform<std::remove_const>;
	using CPoolsCast = ComponentList::template transform<SelectCPool>;

	const auto cpBegin = _cpools.cbegin(), cpMiddle = _cpools.cbegin() + _minIdx,
			   cpMiddleNext = _cpools.cbegin() + _minIdx + 1, cpEnd = _cpools.cend();

	if constexpr (excludeCount != 0) {
		const auto exCpBegin = _exCpools.cbegin(), exCpEnd = _exCpools.cend();
		for (const auto entity : _cpools[_minIdx]->_dense) {
			if (arch::ecs::_details::EntityTraits::Version::hasNotNull(entity) &&
				std::all_of(cpBegin, cpMiddle, [entity](const auto cpool) { return cpool->contains(entity); }) &&
				std::all_of(cpMiddleNext, cpEnd, [entity](const auto cpool) { return cpool->contains(entity); }) &&
				std::none_of(exCpBegin, exCpEnd, [entity](const auto cpool) {
					return cpool && cpool->contains(entity);
				})) {
				if constexpr (PassEntity) {
					fn(entity,
					   reinterpret_cast<CPoolsCast::template get<ComponentList::template find<Cs>>>(
						   _cpools[IncludeTL::template find<Cs>]
					   )
						   ->get(entity)...);
				} else {
					fn(reinterpret_cast<CPoolsCast::template get<ComponentList::template find<Cs>>>(
						   _cpools[IncludeTL::template find<Cs>]
					)
						   ->get(entity)...);
				}
			}
		}
	} else {
		for (const auto entity : _cpools[_minIdx]->_dense) {
			if (arch::ecs::_details::EntityTraits::Version::hasNotNull(entity) &&
				std::all_of(cpBegin, cpMiddle, [entity](const auto cpool) { return cpool->contains(entity); }) &&
				std::all_of(cpMiddleNext, cpEnd, [entity](const auto cpool) { return cpool->contains(entity); })) {
				if constexpr (PassEntity) {
					fn(entity,
					   reinterpret_cast<CPoolsCast::template get<ComponentList::template find<Cs>>>(
						   _cpools[IncludeTL::template find<Cs>]
					   )
						   ->get(entity)...);
				} else {
					fn(reinterpret_cast<CPoolsCast::template get<ComponentList::template find<Cs>>>(
						   _cpools[IncludeTL::template find<Cs>]
					)
						   ->get(entity)...);
				}
			}
		}
	}
}

TEMPLATE_IE
template<class... Cs>
auto VIEW_IE::_all(TypeList<Cs...>) noexcept {
	using ComponentList = TypeList<Cs...>;
	using ActualComponents = ComponentList::template transform<std::remove_const>;
	using CPoolsCast = ComponentList::template transform<SelectCPool>;

	return (*this) | std::views::transform([this](const Entity entity) noexcept {
			   return std::tuple_cat(
				   std::tuple(entity),
				   std::tie(reinterpret_cast<CPoolsCast::template get<ComponentList::template find<Cs>>>(
								_cpools[IncludeTL::template find<Cs>]
				   )
								->get(entity)...)
			   );
		   });
}

TEMPLATE_IE
auto VIEW_IE::all() noexcept {
	if constexpr (includeCount == 1) {
		return std::views::zip(*this, components());
	} else {
		return _all(GetTL());
	}
}

TEMPLATE_IE
template<class... Cs>
auto VIEW_IE::_components(TypeList<Cs...>) noexcept {
	using ComponentList = TypeList<Cs...>;
	using ActualComponents = ComponentList::template transform<std::remove_const>;
	using CPoolsCast = ComponentList::template transform<SelectCPool>;

	return (*this) | std::views::transform([this](const Entity entity) noexcept {
			   return std::tie(reinterpret_cast<CPoolsCast::template get<ComponentList::template find<Cs>>>(
								   _cpools[IncludeTL::template find<Cs>]
			   )
								   ->get(entity)...);
		   });
}

TEMPLATE_IE
auto VIEW_IE::components() noexcept {
	if constexpr (includeCount == 1) {
		using First = GetTL::front;
		static_assert(
			!std::same_as<First, typeList::NoneT>,
			"Cannot call components() while the only component is flag"
		);
		using FirstNonConst = std::remove_const_t<First>;
		using CPool = std::
			conditional_t<std::is_const_v<First>, const ComponentPool<FirstNonConst>*, ComponentPool<FirstNonConst>*>;
		return std::ranges::ref_view(*reinterpret_cast<CPool>(_cpools[0]));
	} else {
		return _components(GetTL());
	}
}

TEMPLATE_IE
template<class... Cs>
auto VIEW_IE::_get(const Entity entity, TypeList<Cs...>) noexcept {
	using ComponentList = TypeList<Cs...>;
	using ActualComponents = ComponentList::template transform<std::remove_const>;
	using CPoolsCast = ComponentList::template transform<SelectCPool>;
	return std::tie(reinterpret_cast<CPoolsCast::template get<ComponentList::template find<Cs>>>(
						_cpools[IncludeTL::template find<Cs>]
	)
						->get(entity)...);
}

TEMPLATE_IE
auto VIEW_IE::get(const Entity entity) noexcept {
	return _get(entity, GetTL());
}

TEMPLATE_IE
bool VIEW_IE::contains(const Entity entity) const noexcept {
	const auto cpBegin = _cpools.cbegin(), cpMiddle = _cpools.cbegin() + _minIdx,
			   cpMiddleNext = _cpools.cbegin() + _minIdx + 1, cpEnd = _cpools.cend();
	if (excludeCount == 0) {
		return arch::ecs::_details::EntityTraits::Version::hasNotNull(entity) &&
			std::all_of(cpBegin, cpMiddle, [entity](const auto cpool) { return cpool->contains(entity); }) &&
			std::all_of(cpMiddleNext, cpEnd, [entity](const auto cpool) { return cpool->contains(entity); });
	} else {
		const auto exCpBegin = _exCpools.cbegin(), exCpEnd = _exCpools.cend();
		return arch::ecs::_details::EntityTraits::Version::hasNotNull(entity) &&
			std::all_of(cpBegin, cpMiddle, [entity](const auto cpool) { return cpool->contains(entity); }) &&
			std::all_of(cpMiddleNext, cpEnd, [entity](const auto cpool) { return cpool->contains(entity); }) &&
			std::none_of(exCpBegin, exCpEnd, [entity](const auto cpool) { return cpool && cpool->contains(entity); });
	}
}

} // namespace arch::ecs

#undef VIEW_IE
#undef TEMPLATE_IE
