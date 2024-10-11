#include <ranges>

#include "View.h"
//
#include "Domain.h"
#include "tUtils/Functions/IsApplicable.h"

#define TEMPLATE_ECIE template<class E, bool Const, class... Includes, class... Excludes>
#define TEMPLATE_ECE template<class E, bool Const, class... Excludes>
#define VIEW_ECIE View<E, Const, TypeList<Includes...>, TypeList<Excludes...>>
#define VIEW_ECE View<E, Const, TypeList<>, TypeList<Excludes...>>

namespace arch::ecs {

namespace _details {

template<class E, class C>
auto getAsTuple(Domain<E>& domain, const E entity) noexcept {
	// make_tuple - by values, tie - by references
	if constexpr (_details::ComponentTraits<C, E>::flag) {
		return std::make_tuple(domain.template getComponent<C>(entity));
	} else {
		return std::tie(domain.template getComponent<C>(entity));
	}
}

template<class E, class C>
auto getAsTuple(const Domain<E>& domain, const E entity) noexcept {
	// make_tuple - by values, tie - by references
	if constexpr (_details::ComponentTraits<C, E>::flag) {
		return std::make_tuple(domain.template getComponent<C>(entity));
	} else {
		return std::tie(domain.template getComponent<C>(entity));
	}
}

template<class E, class... Includes>
auto getByTL(Domain<E>& domain, const E entity, TypeList<Includes...>) noexcept {
	return std::tuple_cat(getAsTuple<E, Includes>(domain, entity)...);
}

template<class E, class... Includes>
auto getByTL(const Domain<E>& domain, const E entity, TypeList<Includes...>) noexcept {
	return std::tuple_cat(getAsTuple<E, Includes>(domain, entity)...);
}

} // namespace _details

TEMPLATE_ECIE
size_t VIEW_ECIE::_minInclude() const noexcept {
	/*ARCH_ASSERT(
		std::ranges::none_of(_includedCPools, [](const auto ptr) { return ptr == nullptr; }),
		"At least one ComponentPool does not exist"
	);*/

	if constexpr (includeCount != 1) {
		const auto min = std::ranges::min_element(_includedCPools, [](const auto lhs, const auto rks) {
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

TEMPLATE_ECIE
template<class C>
auto VIEW_ECIE::getAsTuple(const E entity) noexcept requires(!Const)
{
	constexpr auto idx = Include::template find<C>;

	if constexpr (_details::ComponentTraits<C, E>::flag) {
		return std::make_tuple(
			dynamic_cast<CPoolPtr<C>>(
				const_cast<std::conditional_t<std::is_const_v<C>, CCPoolPtr, _details::CommonComponentPool<E>*>>(
					_includedCPools[idx]
				)
			)
				->get(entity)
		);
	} else {
		return std::tie(
			dynamic_cast<CPoolPtr<C>>(
				const_cast<std::conditional_t<std::is_const_v<C>, CCPoolPtr, _details::CommonComponentPool<E>*>>(
					_includedCPools[idx]
				)
			)
				->get(entity)
		);
	}
}

TEMPLATE_ECIE
template<class C>
auto VIEW_ECIE::getAsTuple(const E entity) const noexcept {
	constexpr auto idx = Include::template find<C>;

	if constexpr (_details::ComponentTraits<C, E>::flag) {
		return std::make_tuple(dynamic_cast<CPoolPtr<C>>(_includedCPools[idx])->get(entity));
	} else {
		return std::tie(dynamic_cast<CPoolPtr<C>>(_includedCPools[idx])->get(entity));
	}
}

TEMPLATE_ECIE
template<class... Cs>
auto VIEW_ECIE::getByTL(const E entity, TypeList<Cs...>) noexcept requires(!Const)
{
	return std::tuple_cat(getAsTuple<Cs>(entity)...);
}

TEMPLATE_ECIE
template<class... Cs>
auto VIEW_ECIE::getByTL(const E entity, TypeList<Cs...>) const noexcept {
	return std::tuple_cat(getAsTuple<Cs>(entity)...);
}

TEMPLATE_ECIE
VIEW_ECIE::View(DomainT* domain) noexcept:
	_includedCPools{ dynamic_cast<CCPoolPtr>(domain->template _tryGetCPool<Includes>())... },
	_excludedCPools{ dynamic_cast<CCPoolPtr>(domain->template _tryGetCPool<Excludes>())... },
	_minIdx{ _minInclude() },
	// can't just call refresh(), _entities is not default_initializable
	_entities(
		_minIdx == (size_t)-1 ? _details::CommonComponentPool<E>::_emptyEntitiesForView() :
								_includedCPools[_minIdx]->_entitiesForView(),
		std::bind(&View::contains, (const View*)this, std::placeholders::_1)
	) {}

TEMPLATE_ECE
VIEW_ECE::View(DomainT* domain) noexcept:
	_excludedCPools{ dynamic_cast<CCPoolPtr>(domain->template _tryGetCPool<Excludes>())... },
	// can't just call refresh(), _entities is not default_initializable
	_entities(domain->entities(), std::bind(&View::_containsNoCheck, this, std::placeholders::_1)),
	_domain{ domain } {
	ARCH_ASSERT(
		std::ranges::none_of(_excludedCPools, [](const auto cpool) { return cpool == nullptr; }),
		"At least one ComponentPool does not exist"
	);
}

TEMPLATE_ECE
bool VIEW_ECE::_containsNoCheck(const E entity) const noexcept {
	return std::ranges::none_of(_excludedCPools, [entity](const auto cpool) { return cpool->contains(entity); });
}

TEMPLATE_ECIE bool VIEW_ECIE::contains(const EntityT entity) const noexcept {
	return std::all_of(
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

TEMPLATE_ECE
bool VIEW_ECE::contains(const EntityT entity) const noexcept {
	return _domain.alive(entity) && _containsNoCheck(entity);
}

TEMPLATE_ECIE VIEW_ECIE& VIEW_ECIE::refresh() noexcept {
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

TEMPLATE_ECE
VIEW_ECE& VIEW_ECE::refresh() noexcept {
	_entities.~EntitesViewT();
	new (&_entities) EntitesViewT(_domain->entities(), std::bind(&View::_containsNoCheck, this, std::placeholders::_1));

	return *this;
}

TEMPLATE_ECIE
auto VIEW_ECIE::get(const EntityT entity) noexcept requires(!Const)
{
	return getByTL(entity, NoFlags());
}

TEMPLATE_ECIE
auto VIEW_ECIE::get(const EntityT entity) const noexcept {
	return getByTL(entity, NoFlags());
}

TEMPLATE_ECIE
template<class... Cs>
auto VIEW_ECIE::get(const EntityT entity) noexcept requires(!Const)
{
	static_assert(
		TypeList<Includes...>::template containsAll<Cs...>,
		"One or more specified components is not contained in this view"
	);
	return getByTL(entity, TypeList<Cs...>());
}

TEMPLATE_ECIE
template<class... Cs>
auto VIEW_ECIE::get(const EntityT entity) const noexcept {
	static_assert(
		TypeList<Includes...>::template containsAll<Cs...>,
		"One or more specified components is not contained in this view"
	);
	return getByTL(entity, TypeList<Cs...>());
}

TEMPLATE_ECIE
auto VIEW_ECIE::getAll(const EntityT entity) noexcept requires(!Const)
{
	return get<Includes...>(entity);
}

TEMPLATE_ECIE
auto VIEW_ECIE::getAll(const EntityT entity) const noexcept {
	return get<Includes...>(entity);
}

TEMPLATE_ECE
template<class... Cs>
auto VIEW_ECE::get(const EntityT entity) noexcept requires(!Const)
{
	return _details::getByTL(*_domain, entity, TypeList<Cs...>());
}

TEMPLATE_ECE
template<class... Cs>
auto VIEW_ECE::get(const EntityT entity) const noexcept {
	return _details::getByTL(*_domain, entity, TypeList<Cs...>());
}

TEMPLATE_ECIE
auto VIEW_ECIE::all() noexcept requires(!Const)
{
	return std::views::transform(_entities, [this](const EntityT entity) {
		return std::tuple_cat(std::make_tuple(entity), get(entity));
	});
}

TEMPLATE_ECIE
auto VIEW_ECIE::all() const noexcept {
	return std::views::transform(_entities, [this](const EntityT entity) {
		return std::tuple_cat(std::make_tuple(entity), get(entity));
	});
}

TEMPLATE_ECIE
auto VIEW_ECIE::components() noexcept requires(!Const)
{
	return std::views::transform(_entities, [this](const EntityT entity) { return getAll(entity); });
}

TEMPLATE_ECIE
auto VIEW_ECIE::components() const noexcept {
	return std::views::transform(_entities, [this](const EntityT entity) { return getAll(entity); });
}

TEMPLATE_ECIE
template<class... Cs>
auto VIEW_ECIE::components() noexcept requires(!Const)
{
	return std::views::transform(_entities, [this](const EntityT entity) { return get<Cs...>(entity); });
}

TEMPLATE_ECIE
template<class... Cs>
auto VIEW_ECIE::components() const noexcept {
	return std::views::transform(_entities, [this](const EntityT entity) { return get<Cs...>(entity); });
}

TEMPLATE_ECIE
template<class Fn>
void VIEW_ECIE::forEach(Fn&& fn) noexcept requires(!Const)
{
	// call modes
	using EntityTuple = decltype(std::tuple<EntityT>());
	using EntityNoFlagsTuple = decltype(std::tuple_cat(std::tuple<EntityT>(), get(EntityT())));
	using EntityComponentsTuple = decltype(std::tuple_cat(std::tuple<EntityT>(), getAll(EntityT())));
	using NoFlagsTuple = decltype(get(EntityT()));
	using ComponentsTuple = decltype(getAll(EntityT()));

	for (const auto entity : _entities) {
		if constexpr (tUtils::isApplicableV<Fn, EntityTuple>) {
			// [...](entity){ ... }
			std::apply(std::forward<Fn>(fn), std::make_tuple(entity));
		} else if constexpr (tUtils::isApplicableV<Fn, EntityNoFlagsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			// non-flags ------ ^ ----- ^ ...
			std::apply(std::forward<Fn>(fn), std::tuple_cat(std::tuple<EntityT>(entity), get(entity)));
		} else if constexpr (tUtils::isApplicableV<Fn, EntityComponentsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			std::apply(std::forward<Fn>(fn), std::tuple_cat(std::tuple<EntityT>(entity), getAll(entity)));
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

TEMPLATE_ECIE
template<class Fn>
void VIEW_ECIE::forEach(Fn&& fn) const noexcept {
	// call modes
	using EntityTuple = decltype(std::tuple<EntityT>());
	using EntityNoFlagsTuple = decltype(std::tuple_cat(std::tuple<EntityT>(), get(EntityT())));
	using EntityComponentsTuple = decltype(std::tuple_cat(std::tuple<EntityT>(), getAll(EntityT())));
	using NoFlagsTuple = decltype(get(EntityT()));
	using ComponentsTuple = decltype(getAll(EntityT()));

	for (const auto entity : _entities) {
		if constexpr (tUtils::isApplicableV<Fn, EntityTuple>) {
			// [...](entity){ ... }
			std::apply(std::forward<Fn>(fn), std::make_tuple(entity));
		} else if constexpr (tUtils::isApplicableV<Fn, EntityNoFlagsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			// non-flags ------ ^ ----- ^ ...
			std::apply(std::forward<Fn>(fn), std::tuple_cat(std::tuple<EntityT>(entity), get(entity)));
		} else if constexpr (tUtils::isApplicableV<Fn, EntityComponentsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			std::apply(std::forward<Fn>(fn), std::tuple_cat(std::tuple<EntityT>(entity), getAll(entity)));
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

TEMPLATE_ECE
template<class Fn>
void VIEW_ECE::forEach(Fn&& fn) noexcept requires(!Const)
{
	// only one call mode for excluding views
	using EntityTuple = decltype(std::tuple<EntityT>());

	for (const auto entity : _entities) {
		if constexpr (tUtils::isApplicableV<Fn, EntityTuple>) {
			// [...](entity){ ... }
			std::apply(std::forward<Fn>(fn), std::make_tuple(entity));
		} else {
			static_assert(false, "Given function is not invocable in any mode, check if argument count == 1");
		}
	}
}

TEMPLATE_ECE
template<class Fn>
void VIEW_ECE::forEach(Fn&& fn) const noexcept {
	// only one call mode for excluding views
	using EntityTuple = decltype(std::tuple<EntityT>());

	for (const auto entity : _entities) {
		if constexpr (tUtils::isApplicableV<Fn, EntityTuple>) {
			// [...](entity){ ... }
			std::apply(std::forward<Fn>(fn), std::make_tuple(entity));
		} else {
			static_assert(false, "Given function is not invocable in any mode, check if argument count == 1");
		}
	}
}

TEMPLATE_ECE
auto VIEW_ECE::begin() noexcept requires(!Const)
{
	return _entities.begin();
}

TEMPLATE_ECE
const auto VIEW_ECE::begin() const noexcept {
	return _entities.begin();
}

TEMPLATE_ECE
const auto VIEW_ECE::cbegin() const noexcept {
	return _entities.begin();
}

TEMPLATE_ECE
auto VIEW_ECE::end() noexcept requires(!Const)
{
	return _entities.end();
}

TEMPLATE_ECE
const auto VIEW_ECE::end() const noexcept {
	return _entities.end();
}

TEMPLATE_ECE
const auto VIEW_ECE::cend() const noexcept {
	return _entities.end();
}

TEMPLATE_ECIE auto VIEW_ECIE::begin() noexcept requires(!Const)
{
	return _entities.begin();
}

TEMPLATE_ECIE
const auto VIEW_ECIE::begin() const noexcept {
	return _entities.begin();
}

TEMPLATE_ECIE
const auto VIEW_ECIE::cbegin() const noexcept {
	return _entities.begin();
}

TEMPLATE_ECIE
auto VIEW_ECIE::end() noexcept requires(!Const)
{
	return _entities.end();
}

TEMPLATE_ECIE
const auto VIEW_ECIE::end() const noexcept {
	return _entities.end();
}

TEMPLATE_ECIE
const auto VIEW_ECIE::cend() const noexcept {
	return _entities.end();
}

} // namespace arch::ecs

#undef TEMPLATE_ECIE
#undef TEMPLATE_ECE
#undef VIEW_ECIE
#undef VIEW_ECE
