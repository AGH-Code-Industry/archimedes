#include "View.h"
//
#include <ranges>

#include "Domain.h"
#include "IsEntity.h"
#include <archimedes/tUtils/LogicalTraits.h>
#include <archimedes/utils/CallableTraits.h>
#include <archimedes/utils/IsApplicable.h>

#define TEMPLATE_IE template<class... Includes, class... Excludes>
#define VIEW_IE View<TypeList<Includes...>, TypeList<Excludes...>>

namespace arch::ecs {

TEMPLATE_IE
VIEW_IE::View(Domain* domain) noexcept:
	_cpools{ (_details::CommonComponentPool*)domain->_tryGetCPool<Includes>()... },
	_excludedCpools{ (_details::CommonComponentPool*)domain->_tryGetCPool<Excludes>()... },
	_minIdx{ _getMinIdx() } {}

TEMPLATE_IE
u32 VIEW_IE::_getMinIdx() const noexcept {
	u32 result = 0;
	for (auto i = 0u; i != _cpools.size(); ++i) {
		if (!_cpools[i] || _cpools[i]->count() == 0) {
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
	if constexpr (includes.size() == 1 && !_details::ComponentTraits<getType<includes.front()>>::inPlace) {
		return Iterator(this, _minIdx == (u32)-1 ? 0 : _cpools[_minIdx]->count(), _minIdx != (u32)-1);
	} else {
		return Iterator(this, _minIdx == (u32)-1 ? 0 : _cpools[_minIdx]->_dense.size(), _minIdx != (u32)-1);
	}
}

TEMPLATE_IE
VIEW_IE::Iterator VIEW_IE::cend() const noexcept {
	return end();
}

TEMPLATE_IE
template<class Fn>
void VIEW_IE::forEach(Fn&& fn) noexcept {
	using Traits = utils::CallableTraits<Fn>;

	if constexpr (Traits::isCallable) {
		// non-template callable, can obtain args

		constexpr auto args = Traits::args;

		constexpr bool isFirstEntity = _details::IsEntity<getType<args.front()>>::value;

		// remove first from Args if entity
		constexpr auto argsNonEntity = args.popFront<isFirstEntity>();

		// utility pred
		constexpr auto isNotConst = [](auto tl) {
			return !std::is_const_v<getType<tl>>;
		};

		// View subset with const components
		constexpr auto availableConst = getTL.filter<std::is_const>();
		// using AvailableConst = GetTL::template filter<std::is_const>;
		//  View subset with non-const components
		constexpr auto availableNonConst = getTL.filter<isNotConst>();
		// using AvailableNonConst = GetTL::template filter<IsNotConst::type>;
		//  View subset with non-const components as consts
		constexpr auto availableNonConstAsConst = availableNonConst.transform<std::add_const>();
		// using AvailableNonConstAsConst = AvailableNonConst::template transform<std::add_const>;
		constexpr auto available = availableConst + availableNonConst + availableNonConstAsConst;
		// using Available = AvailableConst::template cat<AvailableNonConst, AvailableNonConstAsConst>;

		constexpr auto wantedComponent = argsNonEntity.transform<std::remove_reference>();
		// using WantedComponents = ArgsNonEntity::template transform<std::remove_reference>;

		static_assert(available.containsAll(wantedComponent), "Wanted components exceed this view");
		// static_assert(Available::template containsAllFrom<WantedComponents>, "Wanted components exceed this view");

		if (_minIdx == (u32)-1) {
			return;
		}

		_forEach<isFirstEntity>(std::forward<Fn>(fn), wantedComponent);
	} else if constexpr (utils::isApplicableV<
							 Fn,
							 decltype(getTL.transform<std::add_lvalue_reference>())
							 /*typename GetTL::template transform<std::add_lvalue_reference>::toTuple*/>) {
		// if callable with View's components
		_forEach<false>(std::forward<Fn>(fn), getTL);
	} else if constexpr (utils::isApplicableV<
							 Fn,
							 decltype(typelist<Entity> + getTL.transform<std::add_lvalue_reference>())/*typename GetTL::template transform<std::add_lvalue_reference>::template prepend<
								 Entity>::toTuple*/>) {
		// if callable with Entity and View's components
		_forEach<true>(std::forward<Fn>(fn), getTL);
	} else {
		static_assert(false, "fn cannot be called on view");
	}
}

TEMPLATE_IE
template<bool PassEntity, class Fn, class... Cs>
void VIEW_IE::_forEach(Fn&& fn, TypeList<Cs...>) noexcept {
	// component subset
	constexpr auto componentList = typelist<Cs...>;
	constexpr auto actualComponents = componentList.transform<std::remove_const>();
	constexpr auto cpoolsCast = componentList.transform<SelectCPool>();

	const auto cpoolsBegin = _cpools.cbegin(), cpoolsMiddle = _cpools.cbegin() + _minIdx,
			   cpoolsMiddleNext = _cpools.cbegin() + _minIdx + 1, cpoolsEnd = _cpools.cend();

	if constexpr (excludes.size() != 0) {
		const auto excludeCpoolsBegin = _excludedCpools.cbegin(), excludeCpoolsEnd = _excludedCpools.cend();
		for (const auto entity : _cpools[_minIdx]->_dense) {
			// cpools[minIdx] check
			if (arch::ecs::_details::EntityTraits::Version::hasNotNull(entity) &&
				// rest check
				std::all_of(
					cpoolsBegin,
					cpoolsMiddle,
					[entity](const auto cpool) { return cpool->contains(entity); }
				) &&
				std::all_of(
					cpoolsMiddleNext,
					cpoolsEnd,
					[entity](const auto cpool) { return cpool->contains(entity); }
				) &&
				// excludedCpools check
				std::none_of(excludeCpoolsBegin, excludeCpoolsEnd, [entity](const auto cpool) {
					// excluded cpool can be nullptr
					return cpool && cpool->contains(entity);
				})) {
				constexpr auto findCPool = [&]<class T> {
					constexpr auto found = componentList.find<T>();
					constexpr auto cpool = cpoolsCast.get<found>();
					constexpr auto cpoolIdx = includes.find<T>();
					static_assert(cpoolIdx != typelist<>.npos, "type not found");

					return reinterpret_cast<getType<cpool>>(_cpools[]);
				};

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
			// cpools[minIdx] check
			if (arch::ecs::_details::EntityTraits::Version::hasNotNull(entity) &&
				// rest check
				std::all_of(
					cpoolsBegin,
					cpoolsMiddle,
					[entity](const auto cpool) { return cpool->contains(entity); }
				) &&
				std::all_of(cpoolsMiddleNext, cpoolsEnd, [entity](const auto cpool) {
					return cpool->contains(entity);
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
				   std::tie(
					   reinterpret_cast<CPoolsCast::template get<ComponentList::template find<Cs>>>(
						   _cpools[IncludeTL::template find<Cs>]
					   )
						   ->get(entity)...
				   )
			   );
		   });
}

TEMPLATE_IE
auto VIEW_IE::all() noexcept {
	if constexpr (includes.size() == 1) {
		// if view is of one component, returns its cpool
		static_assert(getTL.size() != 0, "Cannot call all() while the only component is flag");

		constexpr auto cpool = getTL.transform<SelectCPool>();

		if (_cpools[0]) {
			return std::views::zip(*this, std::ranges::ref_view(*reinterpret_cast<getType<cpool>>(_cpools[0])));
		} else {
			static std::remove_pointer_t<getType<cpool>> dummy;
			return std::views::zip(*this, std::ranges::ref_view(dummy));
		}
	} else {
		return _all(getTL);
	}
}

TEMPLATE_IE
template<class... Cs>
auto VIEW_IE::_components(TypeList<Cs...>) noexcept {
	using ComponentList = TypeList<Cs...>;
	using ActualComponents = ComponentList::template transform<std::remove_const>;
	using CPoolsCast = ComponentList::template transform<SelectCPool>;

	return (*this) | std::views::transform([this](const Entity entity) noexcept {
			   return std::tie(
				   reinterpret_cast<CPoolsCast::template get<ComponentList::template find<Cs>>>(
					   _cpools[IncludeTL::template find<Cs>]
				   )
					   ->get(entity)...
			   );
		   });
}

TEMPLATE_IE
auto VIEW_IE::components() noexcept {
	if constexpr (includes.size() == 1) {
		// if view is of one component, returns its cpool
		static_assert(getTL.size() != 0, "Cannot call components() while the only component is flag");
		static constexpr auto cpool = getTL.transform<SelectCPool>();
		if (_cpools[0]) {
			return std::ranges::zip_view(std::ranges::ref_view(*reinterpret_cast<getType<cpool>>(_cpools[0])));
		} else {
			static std::remove_pointer_t<getType<cpool>> dummy;
			return std::ranges::zip_view(std::ranges::ref_view(dummy));
		}
	} else {
		return _components(getTL);
	}
}

TEMPLATE_IE
template<class... Cs>
auto VIEW_IE::_get(const Entity entity, TypeList<Cs...>) noexcept {
	using ComponentList = TypeList<Cs...>;
	using ActualComponents = ComponentList::template transform<std::remove_const>;
	using CPoolsCast = ComponentList::template transform<SelectCPool>;
	return std::tie(
		reinterpret_cast<CPoolsCast::template get<ComponentList::template find<Cs>>>(
			_cpools[IncludeTL::template find<Cs>]
		)
			->get(entity)...
	);
}

TEMPLATE_IE
auto VIEW_IE::get(const Entity entity) noexcept {
	return _get(entity, GetTL());
}

TEMPLATE_IE
bool VIEW_IE::contains(const Entity entity) const noexcept {
	const auto cpoolsBegin = _cpools.cbegin(), cpoolsMiddle = _cpools.cbegin() + _minIdx,
			   cpoolsMiddleNext = _cpools.cbegin() + _minIdx + 1, cpoolsEnd = _cpools.cend();
	if constexpr (excludes.size() == 0) {
		return arch::ecs::_details::EntityTraits::Version::hasNotNull(entity) &&
			std::all_of(cpoolsBegin, cpoolsMiddle, [entity](const auto cpool) { return cpool->contains(entity); }) &&
			std::all_of(cpoolsMiddleNext, cpoolsEnd, [entity](const auto cpool) { return cpool->contains(entity); });
	} else {
		const auto excludedCpoolsBegin = _excludedCpools.cbegin(), excludedCpoolsEnd = _excludedCpools.cend();
		return arch::ecs::_details::EntityTraits::Version::hasNotNull(entity) &&
			std::all_of(cpoolsBegin, cpoolsMiddle, [entity](const auto cpool) { return cpool->contains(entity); }) &&
			std::all_of(cpoolsMiddleNext, cpoolsEnd, [entity](const auto cpool) { return cpool->contains(entity); }) &&
			std::none_of(excludedCpoolsBegin, excludedCpoolsEnd, [entity](const auto cpool) {
				   return cpool && cpool->contains(entity);
			   });
	}
}

TEMPLATE_IE
u32 VIEW_IE::minCPool() const noexcept {
	return _minIdx;
}

} // namespace arch::ecs

#undef VIEW_IE
#undef TEMPLATE_IE
