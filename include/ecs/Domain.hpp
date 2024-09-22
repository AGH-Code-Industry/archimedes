#include <ranges>

#include "Domain.h"

#define TEMPLATE_E template<class E>
#define DOMAIN_E Domain<E>

namespace arch::ecs {

TEMPLATE_E
template<class C>
ComponentPool<C, E>& DOMAIN_E::_assureCPool() noexcept {
	const auto type = staticTypedesc(C).wrap(); // custom RTTI gets a use

	auto found = _componentPools.find(type);
	if (found == _componentPools.end()) {
		_cpoolDestroyers[type] = _destroyCPool<C>;
		return *(new (_componentPools[type].storage.data()) ComponentPool<C, E>);
	}

	return *reinterpret_cast<ComponentPool<C, E>*>(found->second.storage.data());
}

TEMPLATE_E
template<class C>
void DOMAIN_E::_destroyCPool(CPoolsT& cpools) noexcept {
	reinterpret_cast<ComponentPool<C, E>*>(cpools[staticTypedesc(C).wrap()].storage.data())->~ComponentPool();
}

TEMPLATE_E
template<class C>
ComponentPool<C, E>& DOMAIN_E::_getCPool() noexcept {
	return *reinterpret_cast<ComponentPool<C, E>*>(_componentPools.find(staticTypedesc(C).wrap())->second.storage.data()
	);
}

TEMPLATE_E
template<class C>
const ComponentPool<C, E>& DOMAIN_E::_getCPool() const noexcept {
	return *reinterpret_cast<const ComponentPool<C, E>*>(
		_componentPools.find(staticTypedesc(C).wrap())->second.storage.data()
	);
}

TEMPLATE_E
template<class C>
ComponentPool<C, E>* DOMAIN_E::_tryGetCPool() noexcept {
	const auto found = _componentPools.find(staticTypedesc(C).wrap());
	return found != _componentPools.end() ? reinterpret_cast<ComponentPool<C, E>*>(found->second.storage.data()) :
											nullptr;
}

TEMPLATE_E template<class C>
const ComponentPool<C, E>* DOMAIN_E::_tryGetCPool() const noexcept {
	const auto found = _componentPools.find(staticTypedesc(C).wrap());
	return found != _componentPools.end() ? reinterpret_cast<const ComponentPool<C, E>*>(found->second.storage.data()) :
											nullptr;
}

TEMPLATE_E
DOMAIN_E::~Domain() noexcept {
	for (const auto& [type, destroyer] : _cpoolDestroyers) {
		destroyer(_componentPools);
	}
}

TEMPLATE_E
void DOMAIN_E::swap(DOMAIN_E& other) noexcept {
	std::swap(_entityPool, other._entityPool);
	std::swap(_componentPools, other._componentPools);
	std::swap(_cpoolDestroyers, other._cpoolDestroyers);
}

TEMPLATE_E
bool DOMAIN_E::alive(const EntityT entity) const noexcept {
	return _entityPool.alive(entity);
}

TEMPLATE_E
bool DOMAIN_E::contains(const IdT id) const noexcept {
	return _entityPool.contains(id);
}

TEMPLATE_E
DOMAIN_E::VersionT DOMAIN_E::version(const EntityT entity) const noexcept {
	return _entityPool.version(entity);
}

TEMPLATE_E
DOMAIN_E::VersionT DOMAIN_E::version(const IdT id) const noexcept {
	return _entityPool.version(id);
}

TEMPLATE_E
size_t DOMAIN_E::count() const noexcept {
	return _entityPool.size();
}

TEMPLATE_E
DOMAIN_E::EntityT DOMAIN_E::newEntity() noexcept {
	return _entityPool.newEntity();
}

TEMPLATE_E
DOMAIN_E::EntityT DOMAIN_E::recycleEntity(const EntityT entity) noexcept {
	return _entityPool.recycleEntity(entity);
}

TEMPLATE_E
DOMAIN_E::EntityT DOMAIN_E::recycleId(const IdT id) noexcept {
	return _entityPool.recycleId(id);
}

TEMPLATE_E
void DOMAIN_E::kill(const EntityT entity) noexcept {
	for (auto&& [type, poolStorage] : _componentPools) {
		reinterpret_cast<ErasableComponentPool<EntityT>*>(&poolStorage)->removeComponent(entity);
	}

	_entityPool.kill(entity);
}

TEMPLATE_E
void DOMAIN_E::kill(std::input_iterator auto first, std::input_iterator auto last) noexcept {
	_entityPool.kill(first, last);
}

TEMPLATE_E
void DOMAIN_E::kill(std::initializer_list<EntityT> entities) noexcept {
	_entityPool.kill(entities);
}

TEMPLATE_E
auto DOMAIN_E::entities() const noexcept {
	return std::views::all(_entityPool);
}

TEMPLATE_E
template<class C, class... Args>
DOMAIN_E::GetReference<C> DOMAIN_E::addComponent(const EntityT entity, Args&&... args) noexcept {
	return _assureCPool<C>().addComponent(entity, std::forward<Args>(args)...);
}

TEMPLATE_E
template<class C>
DOMAIN_E::GetReference<C> DOMAIN_E::getComponent(const EntityT entity) noexcept {
	return _getCPool<C>().get(entity);
}

TEMPLATE_E
template<class C>
DOMAIN_E::ConstGetReference<C> DOMAIN_E::getComponent(const EntityT entity) const noexcept {
	return _getCPool<C>().get(entity);
}

TEMPLATE_E
template<class C>
std::optional<std::reference_wrapper<C>> DOMAIN_E::tryGetComponent(const EntityT entity) noexcept
	requires(not _details::ComponentTraits<C, E>::flag)
{
	return _getCPool<C>().tryGet(entity);
}

TEMPLATE_E
template<class C>
std::optional<std::reference_wrapper<const C>> DOMAIN_E::tryGetComponent(const EntityT entity) noexcept
	requires(not _details::ComponentTraits<C, E>::flag)
{
	return _getCPool<C>().tryGet(entity);
}

TEMPLATE_E
template<class C>
bool DOMAIN_E::removeComponent(const EntityT entity) noexcept {
	return _assureCPool<C>().removeComponent(entity);
}

TEMPLATE_E
template<class C>
C DOMAIN_E::removeComponent(const EntityT entity, MoveFlag) noexcept
	requires(std::movable<C> and not _details::ComponentTraits<C, E>::flag)
{
	return _assureCPool<C>().removeComponent(entity, moveFlag);
}

TEMPLATE_E
template<class C>
requires(not std::is_const_v<C>)
auto DOMAIN_E::components() noexcept {
	return std::views::all(_assureCPool<C>());
}

TEMPLATE_E
template<class C>
requires(std::is_const_v<C>)
auto DOMAIN_E::components() noexcept {
	return std::views::all(
		const_cast<const decltype(_assureCPool<std::remove_const_t<C>>())>(_assureCPool<std::remove_const_t<C>>())
	);
}

TEMPLATE_E
template<class C>
auto DOMAIN_E::components() const noexcept {
	static const ComponentPool<C, E> empty;
	auto poolPtr = _tryGetCPool<C>();
	return poolPtr ? std::views::all(*poolPtr) : std::views::all(empty);
}

TEMPLATE_E
template<class C>
bool DOMAIN_E::hasComponent(const EntityT entity) const noexcept {
	auto cpool = _tryGetCPool<C>();
	return cpool ? cpool->contains(entity) : false;
}

} // namespace arch::ecs

#undef TEMPLATE_E
#undef DOMAIN_E
