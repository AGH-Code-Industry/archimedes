#include <ranges>

#include "Domain.h"

#define TEMPLATE template<class E>
#define DOMAIN Domain<E>

namespace arch::ecs {

TEMPLATE
template<class C>
ComponentPool<C, E>& DOMAIN::_assureCPool() noexcept {
	const auto type = typedesc(C).wrap(); // custom RTTI gets a use

	auto found = _componentPools.find(type);
	if (found == _componentPools.end()) {
		_cpoolDestroyers[type] = _destroyCPool<C>;
		return *(new (_componentPools[type].storage.data()) ComponentPool<C, E>);
	}

	return *reinterpret_cast<ComponentPool<C, E>*>(found->second.storage.data());
}

TEMPLATE
template<class C>
void DOMAIN::_destroyCPool(CPoolsT& cpools) noexcept {
	reinterpret_cast<ComponentPool<C, E>*>(cpools[typedesc(C).wrap()].storage.data())->~ComponentPool();
}

TEMPLATE
template<class C>
ComponentPool<C, E>& DOMAIN::_getCPool() noexcept {
	return *reinterpret_cast<ComponentPool<C, E>*>(_componentPools.find(typedesc(C).wrap())->second.storage.data());
}

TEMPLATE
template<class C>
const ComponentPool<C, E>& DOMAIN::_getCPool() const noexcept {
	return *reinterpret_cast<const ComponentPool<C, E>*>(_componentPools.find(typedesc(C).wrap())->second.storage.data()
	);
}

TEMPLATE
template<class C>
ComponentPool<C, E>* DOMAIN::_tryGetCPool() noexcept {
	const auto found = _componentPools.find(typedesc(C).wrap());
	return found != _componentPools.end() ? reinterpret_cast<ComponentPool<C, E>*>(found->second.storage.data()) :
											nullptr;
}

TEMPLATE template<class C>
const ComponentPool<C, E>* DOMAIN::_tryGetCPool() const noexcept {
	const auto found = _componentPools.find(typedesc(C).wrap());
	return found != _componentPools.end() ? reinterpret_cast<const ComponentPool<C, E>*>(found->second.storage.data()) :
											nullptr;
}

TEMPLATE
DOMAIN::~Domain() noexcept {
	for (const auto& [type, destroyer] : _cpoolDestroyers) {
		destroyer(_componentPools);
	}
}

TEMPLATE
void DOMAIN::swap(DOMAIN& other) noexcept {
	std::swap(_entityPool, other._entityPool);
	std::swap(_componentPools, other._componentPools);
	std::swap(_cpoolDestroyers, other._cpoolDestroyers);
}

TEMPLATE
bool DOMAIN::alive(const EntityT entity) const noexcept {
	return _entityPool.alive(entity);
}

TEMPLATE
bool DOMAIN::contains(const IdT id) const noexcept {
	return _entityPool.contains(id);
}

TEMPLATE
DOMAIN::VersionT DOMAIN::version(const EntityT entity) const noexcept {
	return _entityPool.version(entity);
}

TEMPLATE
DOMAIN::VersionT DOMAIN::version(const IdT id) const noexcept {
	return _entityPool.version(id);
}

TEMPLATE
size_t DOMAIN::count() const noexcept {
	return _entityPool.size();
}

TEMPLATE
DOMAIN::EntityT DOMAIN::newEntity() noexcept {
	return _entityPool.newEntity();
}

TEMPLATE
DOMAIN::EntityT DOMAIN::recycleEntity(const EntityT entity) noexcept {
	return _entityPool.recycleEntity(entity);
}

TEMPLATE
DOMAIN::EntityT DOMAIN::recycleId(const IdT id) noexcept {
	return _entityPool.recycleId(id);
}

TEMPLATE
void DOMAIN::kill(const EntityT entity) noexcept {
	_entityPool.kill(entity);
}

TEMPLATE
void DOMAIN::kill(const IdT id) noexcept {
	_entityPool.kill(id);
}

TEMPLATE
void DOMAIN::kill(std::input_iterator auto first, std::input_iterator auto last) noexcept {
	_entityPool.kill(first, last);
}

TEMPLATE
void DOMAIN::kill(std::initializer_list<EntityT> entities) noexcept {
	_entityPool.kill(entities);
}

TEMPLATE
void DOMAIN::kill(std::initializer_list<IdT> ids) noexcept {
	_entityPool.kill(ids);
}

TEMPLATE
auto DOMAIN::entities() const noexcept {
	return std::views::all(_entityPool);
}

TEMPLATE
template<class C, class... Args>
C& DOMAIN::addComponent(const EntityT entity, Args&&... args) noexcept {
	return _assureCPool<C>().addComponent(entity, std::forward<Args>(args)...);
}

TEMPLATE
template<class C>
C& DOMAIN::getComponent(const EntityT entity) noexcept {
	return _getCPool<C>().get(entity);
}

TEMPLATE
template<class C>
const C& DOMAIN::getComponent(const EntityT entity) const noexcept {
	return _getCPool<C>().get(entity);
}

TEMPLATE
template<class C>
std::optional<std::reference_wrapper<C>> DOMAIN::tryGetComponent(const EntityT entity) noexcept {
	return _getCPool<C>().tryGet(entity);
}

TEMPLATE
template<class C>
std::optional<std::reference_wrapper<const C>> DOMAIN::tryGetComponent(const EntityT entity) noexcept {
	return _getCPool<C>().tryGet(entity);
}

TEMPLATE
template<class C>
bool DOMAIN::removeComponent(const EntityT entity) noexcept {
	return _assureCPool<C>().removeComponent(entity);
}

TEMPLATE
template<class C>
C DOMAIN::removeComponent(const EntityT entity, MoveFlag) noexcept requires std::movable<C>
{
	return _assureCPool<C>().removeComponent(entity, moveFlag);
}

TEMPLATE
template<class C>
requires(not std::is_const_v<C>)
auto DOMAIN::components() noexcept {
	return std::views::all(_assureCPool<C>());
}

TEMPLATE
template<class C>
requires(std::is_const_v<C>)
auto DOMAIN::components() noexcept {
	return std::views::all(
		const_cast<const decltype(_assureCPool<std::remove_const_t<C>>())>(_assureCPool<std::remove_const_t<C>>())
	);
}

TEMPLATE
template<class C>
auto DOMAIN::components() const noexcept {
	static const ComponentPool<C, E> empty;
	auto poolPtr = _tryGetCPool<C>();
	return poolPtr ? std::views::all(*poolPtr) : std::views::all(empty);
}

TEMPLATE
template<class C>
bool DOMAIN::hasComponent(const EntityT entity) const noexcept {
	auto cpool = _tryGetCPool<C>();
	return cpool ? cpool->contains(entity) : false;
}

} // namespace arch::ecs

#undef TEMPLATE
#undef DOMAIN
