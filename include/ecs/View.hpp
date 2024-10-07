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
inline auto getAsTuple(Domain<E>& domain, const E entity) noexcept {
	// make_tuple - by values, tie - by references
	if constexpr (_details::ComponentTraits<C, E>::flag) {
		return std::make_tuple(domain.template getComponent<C>(entity));
	} else {
		return std::tie(domain.template getComponent<C>(entity));
	}
}

template<class E, class C>
inline auto getAsTuple(const Domain<E>& domain, const E entity) noexcept {
	// make_tuple - by values, tie - by references
	if constexpr (_details::ComponentTraits<C, E>::flag) {
		return std::make_tuple(domain.template getComponent<C>(entity));
	} else {
		return std::tie(domain.template getComponent<C>(entity));
	}
}

template<class E, class... Includes>
inline auto getByTS(Domain<E>& domain, const E entity, TypeList<Includes...>) noexcept {
	return std::tuple_cat(getAsTuple<E, Includes>(domain, entity)...);
}

template<class E, class... Includes>
inline auto getByTS(const Domain<E>& domain, const E entity, TypeList<Includes...>) noexcept {
	return std::tuple_cat(getAsTuple<E, Includes>(domain, entity)...);
}

} // namespace _details

TEMPLATE_ECIE
VIEW_ECIE::View(DomainT* domain, const _details::CommonComponentPool<E>& minCPool) noexcept:
	_domain{ domain },
	// can't just call refresh(), _entities is not default_initializable
	_entities(minCPool._entitiesForView(), std::bind(_filterFn, std::cref(*domain), std::placeholders::_1)) {}

TEMPLATE_ECIE
bool VIEW_ECIE::_filterFn(const Domain<E>& domain, const E entity) noexcept {
	return (domain.template hasComponent<std::remove_const_t<Includes>>(entity) && ...) &&
		!(domain.template hasComponent<std::remove_const_t<Excludes>>(entity) || ...);
}

TEMPLATE_ECE
VIEW_ECE::View(DomainT* domain) noexcept:
	_domain{ domain },
	// can't just call refresh(), _entities is not default_initializable
	_entities(domain->entities(), std::bind(_filterFn, std::cref(*domain), std::placeholders::_1)) {}

TEMPLATE_ECE
bool VIEW_ECE::_filterFn(const Domain<E>& domain, const E entity) noexcept {
	return !(domain.template hasComponent<std::remove_const_t<Excludes>>(entity) || ...);
}

TEMPLATE_ECIE
bool VIEW_ECIE::contains(const EntityT entity) const noexcept {
	return (_domain->template hasComponent<std::remove_const_t<Includes>>(entity) && ...) &&
		!(_domain->template hasComponent<std::remove_const_t<Excludes>>(entity) || ...);
}

TEMPLATE_ECE
bool VIEW_ECE::contains(const EntityT entity) const noexcept {
	return _domain.alive(entity) && !(_domain->template hasComponent<std::remove_const_t<Excludes>>(entity) || ...);
}

TEMPLATE_ECIE
VIEW_ECIE& VIEW_ECIE::refresh() noexcept {
	ARCH_ASSERT(
		std::ranges::none_of(
			std::initializer_list<const _details::CommonComponentPool<E>*>{
				dynamic_cast<const _details::CommonComponentPool<E>*>(
					_domain->_tryGetCPool<std::remove_const_t<Includes>>()
				)... },
			[](const auto ptr) { return ptr == nullptr; }
		),
		"One of requested ComponentPools does not exist"
	);
	// destroy _entities
	_entities.~EntitesViewT();
	const auto minCPool = std::min(
		{ dynamic_cast<const _details::CommonComponentPool<E>*>(&_domain->_getCPool<std::remove_const_t<Includes>>()
		)... },
		[](const auto lhs, const auto rks) { return lhs->count() < rks->count(); }
	);
	// create new _entites in place of deleted
	new (&_entities)
		EntitesViewT(minCPool->_entitiesForView(), std::bind(_filterFn, std::cref(*_domain), std::placeholders::_1));

	return *this;
}

TEMPLATE_ECE
VIEW_ECE& VIEW_ECE::refresh() noexcept {
	_entities.~EntitesViewT();
	new (&_entities)
		EntitesViewT(_domain->entities(), std::bind(_filterFn, std::cref(*_domain), std::placeholders::_1));

	return *this;
}

TEMPLATE_ECIE
auto VIEW_ECIE::get(const EntityT entity) noexcept requires(!Const)
{
	return _details::getByTS(*_domain, entity, NoFlags());
}

TEMPLATE_ECIE
auto VIEW_ECIE::get(const EntityT entity) const noexcept {
	return _details::getByTS(*_domain, entity, NoFlags());
}

TEMPLATE_ECIE
template<class... Cs>
auto VIEW_ECIE::get(const EntityT entity) noexcept requires(!Const)
{
	return _details::getByTS(*_domain, entity, TypeList<Cs...>());
}

TEMPLATE_ECIE
template<class... Cs>
auto VIEW_ECIE::get(const EntityT entity) const noexcept {
	return _details::getByTS(*_domain, entity, TypeList<Cs...>());
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
	return _details::getByTS(*_domain, entity, TypeList<Cs...>());
}

TEMPLATE_ECE
template<class... Cs>
auto VIEW_ECE::get(const EntityT entity) const noexcept {
	return _details::getByTS(*_domain, entity, TypeList<Cs...>());
}

TEMPLATE_ECIE
auto VIEW_ECIE::all() noexcept requires(!Const)
{
	return std::views::transform(_entities, [domain = _domain](const EntityT entity) {
		return std::tuple_cat(std::make_tuple(entity), _details::getByTS(*domain, entity, NoFlags()));
	});
}

TEMPLATE_ECIE
auto VIEW_ECIE::all() const noexcept {
	return std::views::transform(_entities, [domain = _domain](const EntityT entity) {
		return std::tuple_cat(std::make_tuple(entity), _details::getByTS(*domain, entity, NoFlags()));
	});
}

TEMPLATE_ECIE
auto VIEW_ECIE::components() noexcept requires(!Const)
{
	return std::views::transform(_entities, [domain = _domain](const EntityT entity) {
		return _details::getByTS(*domain, entity, NoFlags());
	});
}

TEMPLATE_ECIE
auto VIEW_ECIE::components() const noexcept {
	return std::views::transform(_entities, [domain = _domain](const EntityT entity) {
		return _details::getByTS(*domain, entity, NoFlags());
	});
}

TEMPLATE_ECIE
template<class... Cs>
auto VIEW_ECIE::components() noexcept requires(!Const)
{
	static_assert(
		TypeList<Includes...>::template containsAll<Cs...>,
		"One or more specified components is not contained in this view"
	);

	return std::views::transform(_entities, [domain = _domain](const EntityT entity) {
		return _details::getByTS(*domain, entity, TypeList<Cs...>());
	});
}

TEMPLATE_ECIE
template<class... Cs>
auto VIEW_ECIE::components() const noexcept {
	static_assert(
		TypeList<Includes...>::template containsAll<Cs...>,
		"One or more specified components is not contained in this view"
	);

	return std::views::transform(_entities, [domain = _domain](const EntityT entity) {
		return _details::getByTS(*domain, entity, TypeList<Cs...>());
	});
}

TEMPLATE_ECIE
template<class Fn>
void VIEW_ECIE::forEach(Fn&& fn) noexcept requires(!Const)
{
	// call modes
	using EntityTuple = decltype(std::tuple<EntityT>());
	using EntityNoFlagsTuple =
		decltype(std::tuple_cat(std::tuple<EntityT>(), _details::getByTS(*_domain, EntityT(), NoFlags())));
	using EntityComponentsTuple =
		decltype(std::tuple_cat(std::tuple<EntityT>(), _details::getByTS(*_domain, EntityT(), Include())));
	using NoFlagsTuple = decltype(_details::getByTS(*_domain, EntityT(), NoFlags()));
	using ComponentsTuple = decltype(_details::getByTS(*_domain, EntityT(), Include()));

	for (const auto entity : _entities) {
		if constexpr (tUtils::isApplicableV<Fn, EntityTuple>) {
			// [...](entity){ ... }
			std::apply(std::forward<Fn>(fn), std::make_tuple(entity));
		} else if constexpr (tUtils::isApplicableV<Fn, EntityNoFlagsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			// non-flags ------ ^ ----- ^ ...
			std::apply(
				std::forward<Fn>(fn),
				std::tuple_cat(std::tuple<EntityT>(entity), _details::getByTS(*_domain, entity, NoFlags()))
			);
		} else if constexpr (tUtils::isApplicableV<Fn, EntityComponentsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			std::apply(
				std::forward<Fn>(fn),
				std::tuple_cat(std::tuple<EntityT>(entity), _details::getByTS(*_domain, entity, Include()))
			);
		} else if constexpr (tUtils::isApplicableV<Fn, NoFlagsTuple>) {
			// [...](      comp1&, comp2&, ...){ ... }
			// non-flags ---- ^ ----- ^ ...
			std::apply(std::forward<Fn>(fn), _details::getByTS(*_domain, entity, NoFlags()));
		} else if constexpr (tUtils::isApplicableV<Fn, ComponentsTuple>) {
			// [...](comp1&, comp2&, ...){ ... }
			std::apply(std::forward<Fn>(fn), _details::getByTS(*_domain, entity, Include()));
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
	using EntityNoFlagsTuple =
		decltype(std::tuple_cat(std::tuple<EntityT>(), _details::getByTS(*_domain, EntityT(), NoFlags())));
	using EntityComponentsTuple =
		decltype(std::tuple_cat(std::tuple<EntityT>(), _details::getByTS(*_domain, EntityT(), Include())));
	using NoFlagsTuple = decltype(_details::getByTS(*_domain, EntityT(), NoFlags()));
	using ComponentsTuple = decltype(_details::getByTS(*_domain, EntityT(), Include()));

	for (const auto entity : _entities) {
		((void)0);
		if constexpr (tUtils::isApplicableV<Fn, EntityTuple>) {
			// [...](entity){ ... }
			std::apply(std::forward<Fn>(fn), std::make_tuple(entity));
		} else if constexpr (tUtils::isApplicableV<Fn, EntityNoFlagsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			// non-flags ------ ^ ----- ^ ...
			std::apply(
				std::forward<Fn>(fn),
				std::tuple_cat(std::tuple<EntityT>(entity), _details::getByTS(*_domain, entity, NoFlags()))
			);
		} else if constexpr (tUtils::isApplicableV<Fn, EntityComponentsTuple>) {
			// [...](entity, comp1&, comp2&, ...){ ... }
			std::apply(
				std::forward<Fn>(fn),
				std::tuple_cat(std::tuple<EntityT>(entity), _details::getByTS(*_domain, entity, Include()))
			);
		} else if constexpr (tUtils::isApplicableV<Fn, NoFlagsTuple>) {
			// [...](      comp1&, comp2&, ...){ ... }
			// non-flags ---- ^ ----- ^ ...
			std::apply(std::forward<Fn>(fn), _details::getByTS(*_domain, entity, NoFlags()));
		} else if constexpr (tUtils::isApplicableV<Fn, ComponentsTuple>) {
			// [...](comp1&, comp2&, ...){ ... }
			std::apply(std::forward<Fn>(fn), _details::getByTS(*_domain, entity, Include()));
		} else {
			static_assert(false, "Given function is not invocable in any forEach mode, check if argument counts match");
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
