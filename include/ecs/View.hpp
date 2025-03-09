#include <algorithm>
#include <ranges>

#include "Domain.h"
//
#include "View.h"
#include "tUtils/Functions/IsApplicable.h"

#define TEMPLATE_CIE template<bool Const, class... Includes, class... Excludes>
#define TEMPLATE_CE template<bool Const, class... Excludes>
#define VIEW_CIE View<Const, TypeList<Includes...>, TypeList<Excludes...>>
#define VIEW_CE View<Const, TypeList<>, TypeList<Excludes...>>

namespace arch::ecs {

namespace _details {

template<class C>
auto getAsTuple(Domain& domain, const Entity entity) noexcept {
	// make_tuple - by values, tie - by references
	if constexpr (_details::ComponentTraits<C>::flag) {
		return std::make_tuple(domain.template getComponent<C>(entity));
	} else {
		return std::tie(domain.template getComponent<C>(entity));
	}
}

template<class C>
auto getAsTuple(const Domain& domain, const Entity entity) noexcept {
	// make_tuple - by values, tie - by references
	if constexpr (_details::ComponentTraits<C>::flag) {
		return std::make_tuple(domain.template getComponent<C>(entity));
	} else {
		return std::tie(domain.template getComponent<C>(entity));
	}
}

template<class... Includes>
auto getByTL(Domain& domain, const Entity entity, TypeList<Includes...>) noexcept {
	return std::tuple_cat(getAsTuple<Includes>(domain, entity)...);
}

template<class... Includes>
auto getByTL(const Domain& domain, const Entity entity, TypeList<Includes...>) noexcept {
	return std::tuple_cat(getAsTuple<Includes>(domain, entity)...);
}

} // namespace _details

TEMPLATE_CIE
size_t VIEW_CIE::_minInclude() const noexcept {
	if constexpr (includeCount != 1) {
		const auto min = std::ranges::min_element(_includedCPools, [&](const auto lhs, const auto rks) {
			return !lhs ? true : (!rks ? true : lhs->count() < rks->count());
		});
		if (!(*min)) { // at least one cpool is empty
			return (size_t)-1;
		}

		auto dist = std::ranges::distance(_includedCPools.begin(), min);
		return dist;
	} else {
		return _includedCPools[0] ? 0 : (size_t)-1;
	}
}

TEMPLATE_CIE
template<class C>
auto VIEW_CIE::getAsTuple(const Entity entity) noexcept requires(!Const)
{
	constexpr auto idx = Include::template find<C>;

	if constexpr (_details::ComponentTraits<C>::flag) {
		return std::make_tuple(
			dynamic_cast<CPoolPtr<C>>(
				const_cast<std::conditional_t<std::is_const_v<C>, CCPoolPtr, _details::CommonComponentPool*>>(
					_includedCPools[idx]
				)
			)
				->get(entity)
		);
	} else {
		return std::tie(
			dynamic_cast<CPoolPtr<C>>(
				const_cast<std::conditional_t<std::is_const_v<C>, CCPoolPtr, _details::CommonComponentPool*>>(
					_includedCPools[idx]
				)
			)
				->get(entity)
		);
	}
}

TEMPLATE_CIE
template<class C>
auto VIEW_CIE::getAsTuple(const Entity entity) const noexcept {
	constexpr auto idx = Include::template find<C>;

	if constexpr (_details::ComponentTraits<C>::flag) {
		return std::make_tuple(dynamic_cast<CPoolPtr<C>>(_includedCPools[idx])->get(entity));
	} else {
		return std::tie(dynamic_cast<CPoolPtr<C>>(_includedCPools[idx])->get(entity));
	}
}

TEMPLATE_CIE
template<class... Cs>
auto VIEW_CIE::getByTL(const Entity entity, TypeList<Cs...>) noexcept requires(!Const)
{
	return std::tuple_cat(getAsTuple<Cs>(entity)...);
}

TEMPLATE_CIE
template<class... Cs>
auto VIEW_CIE::getByTL(const Entity entity, TypeList<Cs...>) const noexcept {
	return std::tuple_cat(getAsTuple<Cs>(entity)...);
}

TEMPLATE_CIE
VIEW_CIE::View(DomainT* domain) noexcept:
	_includedCPools{ dynamic_cast<CCPoolPtr>(domain->template _tryGetCPool<Includes>())... },
	_excludedCPools{ dynamic_cast<CCPoolPtr>(domain->template _tryGetCPool<Excludes>())... },
	_minIdx{ _minInclude() },
	// can't just call refresh(), _entities is not default_initializable
	_entities(
		_minIdx == (size_t)-1 ? _details::CommonComponentPool::_emptyEntitiesForView() :
								_includedCPools[_minIdx]->_entitiesForView(),
		std::bind(&View::contains, (const View*)this, std::placeholders::_1)
	),
	_domain{ domain } {}

TEMPLATE_CIE
VIEW_CIE::View(const View& other) noexcept:
	_includedCPools{ other._includedCPools },
	_excludedCPools{ other._excludedCPools },
	_minIdx{ other._minIdx },
	_entities{ other._entities.base(), std::bind(&View::contains, (const View*)this, std::placeholders::_1) } {}

TEMPLATE_CIE
VIEW_CIE::View(View&& other) noexcept:
	_includedCPools{ std::move(other._includedCPools) },
	_excludedCPools{ std::move(other._excludedCPools) },
	_minIdx{ std::move(other._minIdx) },
	_entities{ std::move(other._entities).base(),
			   std::bind(&View::contains, (const View*)this, std::placeholders::_1) } {
	other._includedCPools = {};
	other._excludedCPools = {};
	other._minIdx = (size_t)-1;
	other._entities = std::views::filter(
		ecs::_details::CommonComponentPool::_emptyEntitiesForView(),
		std::bind(&View::contains, (const View*)&other, std::placeholders::_1)
	);
}

TEMPLATE_CIE
VIEW_CIE& VIEW_CIE::operator=(const View& other) noexcept {
	_includedCPools = other._includedCPools;
	_excludedCPools = other._excludedCPools;
	_minIdx = other._minIdx;
	_entities = std::ranges::filter_view(
		other._entities.base(),
		std::bind(&View::contains, (const View*)this, std::placeholders::_1)
	);
}

TEMPLATE_CIE
VIEW_CIE& VIEW_CIE::operator=(View&& other) noexcept {
	_includedCPools = std::move(other._includedCPools);
	_excludedCPools = std::move(other._excludedCPools);
	_minIdx = std::move(other._minIdx);
	_entities = std::ranges::filter_view(
		std::move(other._entities).base(),
		std::bind(&View::contains, (const View*)this, std::placeholders::_1)
	);

	other._includedCPools = {};
	other._excludedCPools = {};
	other._minIdx = (size_t)-1;
	other._entities = std::views::filter(
		ecs::_details::CommonComponentPool::_emptyEntitiesForView(),
		std::bind(&View::contains, (const View*)&other, std::placeholders::_1)
	);
}

TEMPLATE_CE VIEW_CE::View(DomainT* domain) noexcept:
	_excludedCPools{ dynamic_cast<CCPoolPtr>(domain -> template _tryGetCPool<Excludes>())... },
	// can't just call refresh(), _entities is not default_initializable
	_entities(domain->entities(), std::bind(&View::_containsNoCheck, (const View*)this, std::placeholders::_1)),
	_domain{ domain } {
	ARCH_ASSERT(
		std::ranges::none_of(_excludedCPools, [](const auto cpool) { return cpool == nullptr; }),
		"At least one ComponentPool does not exist"
	);
}

TEMPLATE_CE
VIEW_CE::View(const View& other) noexcept:
	_excludedCPools{ other._excludedCPools },
	_entities{ other._domain->entities(),
			   std::bind(&View::_containsNoCheck, (const View*)this, std::placeholders::_1) },
	_domain{ other._domain } {}

TEMPLATE_CE
VIEW_CE::View(View&& other) noexcept:
	_excludedCPools{ std::move(other._excludedCPools) },
	_entities{ std::move(other._domain->entities()),
			   std::bind(&View::_containsNoCheck, (const View*)this, std::placeholders::_1) },
	_domain{ std::move(other._domain) } {
	other._excludedCPools = {};
	other._domain = {};
}

TEMPLATE_CE
bool VIEW_CE::_containsNoCheck(const Entity entity) const noexcept {
	return std::ranges::none_of(_excludedCPools, [entity](const auto cpool) { return cpool->contains(entity); });
}

TEMPLATE_CIE
bool VIEW_CIE::contains(const Entity entity) const noexcept {
	return _domain->alive(entity) &&
		std::all_of(
			   _includedCPools.begin(),
			   _includedCPools.begin() + _minIdx,
			   [entity](const auto cpool) { return cpool->contains(entity); }
		) &&
		std::all_of(
			   _includedCPools.begin() + _minIdx + 1,
			   _includedCPools.end(),
			   [entity](const auto cpool) { return cpool->contains(entity); }
		) &&
		std::ranges::none_of(_excludedCPools, [entity](const auto cpool) { return cpool->contains(entity); });
}

TEMPLATE_CE
bool VIEW_CE::contains(const Entity entity) const noexcept {
	return _domain.alive(entity) && _containsNoCheck(entity);
}

TEMPLATE_CIE VIEW_CIE& VIEW_CIE::refresh() noexcept {
	// destroy _entities
	_entities.~EntitesViewT();
	_minIdx = _minInclude();
	// create new _entites in place of deleted
	new (&_entities) EntitesViewT(
		_includedCPools[_minIdx]->_entitiesForView(),
		std::bind(&View::contains, (const View*)this, std::placeholders::_1)
	);

	return *this;
}

TEMPLATE_CE
VIEW_CE& VIEW_CE::refresh() noexcept {
	_entities.~EntitesViewT();
	new (&_entities)
		EntitesViewT(_domain->entities(), std::bind(&View::_containsNoCheck, (const View*)this, std::placeholders::_1));

	return *this;
}

TEMPLATE_CIE
auto VIEW_CIE::get(const Entity entity) noexcept requires(!Const)
{
	return getByTL(entity, NoFlags());
}

TEMPLATE_CIE
auto VIEW_CIE::get(const Entity entity) const noexcept {
	return getByTL(entity, NoFlags());
}

TEMPLATE_CIE
template<class... Cs>
auto VIEW_CIE::get(const Entity entity) noexcept requires(!Const)
{
	static_assert(
		TypeList<Includes...>::template containsAll<Cs...>,
		"One or more specified components is not contained in this view"
	);
	return getByTL(entity, TypeList<Cs...>());
}

TEMPLATE_CIE
template<class... Cs>
auto VIEW_CIE::get(const Entity entity) const noexcept {
	static_assert(
		TypeList<Includes...>::template containsAll<Cs...>,
		"One or more specified components is not contained in this view"
	);
	return getByTL(entity, TypeList<Cs...>());
}

TEMPLATE_CIE
auto VIEW_CIE::getAll(const Entity entity) noexcept requires(!Const)
{
	return get<Includes...>(entity);
}

TEMPLATE_CIE
auto VIEW_CIE::getAll(const Entity entity) const noexcept {
	return get<Includes...>(entity);
}

TEMPLATE_CE
template<class... Cs>
auto VIEW_CE::get(const Entity entity) noexcept requires(!Const)
{
	return _details::getByTL(*_domain, entity, TypeList<Cs...>());
}

TEMPLATE_CE
template<class... Cs>
auto VIEW_CE::get(const Entity entity) const noexcept {
	return _details::getByTL(*_domain, entity, TypeList<Cs...>());
}

TEMPLATE_CIE
auto VIEW_CIE::all() noexcept requires(!Const)
{
	return std::views::transform(_entities, [this](const Entity entity) {
		return std::tuple_cat(std::make_tuple(entity), get(entity));
	});
}

TEMPLATE_CIE
auto VIEW_CIE::all() const noexcept {
	return std::views::transform(_entities, [this](const Entity entity) {
		return std::tuple_cat(std::make_tuple(entity), get(entity));
	});
}

TEMPLATE_CIE
auto VIEW_CIE::components() noexcept requires(!Const)
{
	return std::views::transform(_entities, [this](const Entity entity) { return getAll(entity); });
}

TEMPLATE_CIE
auto VIEW_CIE::components() const noexcept {
	return std::views::transform(_entities, [this](const Entity entity) { return getAll(entity); });
}

TEMPLATE_CIE
template<class... Cs>
auto VIEW_CIE::components() noexcept requires(!Const)
{
	return std::views::transform(_entities, [this](const Entity entity) { return get<Cs...>(entity); });
}

TEMPLATE_CIE
template<class... Cs>
auto VIEW_CIE::components() const noexcept {
	return std::views::transform(_entities, [this](const Entity entity) { return get<Cs...>(entity); });
}

TEMPLATE_CIE
template<class Fn>
void VIEW_CIE::forEach(Fn&& fn) noexcept requires(!Const)
{
	// call modes
	using EntityTuple = decltype(std::tuple<Entity>());
	using EntityNoFlagsTuple = decltype(std::tuple_cat(std::tuple<Entity>(), get(Entity())));
	using EntityComponentsTuple = decltype(std::tuple_cat(std::tuple<Entity>(), getAll(Entity())));
	using NoFlagsTuple = decltype(get(Entity()));
	using ComponentsTuple = decltype(getAll(Entity()));

	for (const auto entity : _entities) {
		if constexpr (tUtils::isApplicableV<Fn, EntityTuple>) {
			// [...](entity){ ... }
			std::apply(std::forward<Fn>(fn), std::make_tuple(entity));
		} else if constexpr (tUtils::isApplicableV<Fn, EntityNoFlagsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			// non-flags ------ ^ ----- ^ ...
			std::apply(std::forward<Fn>(fn), std::tuple_cat(std::tuple<Entity>(entity), get(entity)));
		} else if constexpr (tUtils::isApplicableV<Fn, EntityComponentsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			std::apply(std::forward<Fn>(fn), std::tuple_cat(std::tuple<Entity>(entity), getAll(entity)));
		} else if constexpr (tUtils::isApplicableV<Fn, NoFlagsTuple>) {
			// [...](      comp1&, comp2&, ...){ ... }
			// non-flags ---- ^ ----- ^ ...
			std::apply(std::forward<Fn>(fn), get(entity));
		} else if constexpr (tUtils::isApplicableV<Fn, ComponentsTuple>) {
			// [...](comp1&, comp2&, ...){ ... }
			std::apply(std::forward<Fn>(fn), getAll(entity));
		} else {
			static_assert(false, "Given function is not invocable in any forEach mode");
		}
	}
}

TEMPLATE_CIE
template<class Fn>
void VIEW_CIE::forEach(Fn&& fn) const noexcept {
	// call modes
	using EntityTuple = decltype(std::tuple<Entity>());
	using EntityNoFlagsTuple = decltype(std::tuple_cat(std::tuple<Entity>(), get(Entity())));
	using EntityComponentsTuple = decltype(std::tuple_cat(std::tuple<Entity>(), getAll(Entity())));
	using NoFlagsTuple = decltype(get(Entity()));
	using ComponentsTuple = decltype(getAll(Entity()));

	for (const auto entity : _entities) {
		if constexpr (tUtils::isApplicableV<Fn, EntityTuple>) {
			// [...](entity){ ... }
			std::apply(std::forward<Fn>(fn), std::make_tuple(entity));
		} else if constexpr (tUtils::isApplicableV<Fn, EntityNoFlagsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			// non-flags ------ ^ ----- ^ ...
			std::apply(std::forward<Fn>(fn), std::tuple_cat(std::tuple<Entity>(entity), get(entity)));
		} else if constexpr (tUtils::isApplicableV<Fn, EntityComponentsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			std::apply(std::forward<Fn>(fn), std::tuple_cat(std::tuple<Entity>(entity), getAll(entity)));
		} else if constexpr (tUtils::isApplicableV<Fn, NoFlagsTuple>) {
			// [...](      comp1&, comp2&, ...){ ... }
			// non-flags ---- ^ ----- ^ ...
			std::apply(std::forward<Fn>(fn), get(entity));
		} else if constexpr (tUtils::isApplicableV<Fn, ComponentsTuple>) {
			// [...](comp1&, comp2&, ...){ ... }
			std::apply(std::forward<Fn>(fn), getAll(entity));
		} else {
			static_assert(false, "Given function is not invocable in any forEach mode");
		}
	}
}

TEMPLATE_CE
template<class Fn>
void VIEW_CE::forEach(Fn&& fn) noexcept requires(!Const)
{
	// only one call mode for excluding views
	using EntityTuple = decltype(std::tuple<Entity>());

	for (const auto entity : _entities) {
		if constexpr (tUtils::isApplicableV<Fn, EntityTuple>) {
			// [...](entity){ ... }
			std::apply(std::forward<Fn>(fn), std::make_tuple(entity));
		} else {
			static_assert(false, "Given function is not invocable in any mode, check if argument count == 1");
		}
	}
}

TEMPLATE_CE
template<class Fn>
void VIEW_CE::forEach(Fn&& fn) const noexcept {
	// only one call mode for excluding views
	using EntityTuple = decltype(std::tuple<Entity>());

	for (const auto entity : _entities) {
		if constexpr (tUtils::isApplicableV<Fn, EntityTuple>) {
			// [...](entity){ ... }
			std::apply(std::forward<Fn>(fn), std::make_tuple(entity));
		} else {
			static_assert(false, "Given function is not invocable in any mode, check if argument count == 1");
		}
	}
}

// TEMPLATE_CE
// auto VIEW_CE::begin() noexcept requires(!Const)
//{
//	return _entities.begin();
// }

TEMPLATE_CE
auto VIEW_CE::begin() const noexcept {
	return const_cast<VIEW_CE*>(this)->_entities.begin();
}

TEMPLATE_CE
auto VIEW_CE::cbegin() const noexcept {
	return const_cast<VIEW_CE*>(this)->_entities.begin();
}

// TEMPLATE_CE
// auto VIEW_CE::end() noexcept requires(!Const)
//{
//	return _entities.end();
// }

TEMPLATE_CE
auto VIEW_CE::end() const noexcept {
	return const_cast<VIEW_CE*>(this)->_entities.end();
}

TEMPLATE_CE
auto VIEW_CE::cend() const noexcept {
	return const_cast<VIEW_CE*>(this)->_entities.end();
}

// TEMPLATE_CIE auto VIEW_CIE::begin() noexcept requires(!Const)
//{
//	return _entities.begin();
// }

TEMPLATE_CIE
auto VIEW_CIE::begin() const noexcept {
	return const_cast<VIEW_CIE*>(this)->_entities.begin();
}

TEMPLATE_CIE
auto VIEW_CIE::cbegin() const noexcept {
	return const_cast<VIEW_CIE*>(this)->_entities.begin();
}

// TEMPLATE_CIE
// auto VIEW_CIE::end() noexcept requires(!Const)
//{
//	return _entities.end();
// }

TEMPLATE_CIE
auto VIEW_CIE::end() const noexcept {
	return const_cast<VIEW_CIE*>(this)->_entities.end();
}

TEMPLATE_CIE
auto VIEW_CIE::cend() const noexcept {
	return const_cast<VIEW_CIE*>(this)->_entities.end();
}

} // namespace arch::ecs

#undef TEMPLATE_CIE
#undef TEMPLATE_CE
#undef VIEW_CIE
#undef VIEW_CE
