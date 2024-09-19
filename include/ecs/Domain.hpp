#include <ranges>

#include "Domain.h"

#define TEMPLATE template<class E>
#define TEMP_DOMAIN Domain<E>

namespace arch::ecs {

TEMPLATE
template<class C>
ComponentPool<C, E>& TEMP_DOMAIN::_assureCPool() noexcept {
	const auto type = staticTypedesc(C).wrap(); // custom RTTI gets a use

	auto found = _componentPools.find(type);
	if (found == _componentPools.end()) {
		_cpoolDestroyers[type] = _destroyCPool<C>;
		return *(new (_componentPools[type].storage.data()) ComponentPool<C, E>);
	}

	return *reinterpret_cast<ComponentPool<C, E>*>(found->second.storage.data());
}

TEMPLATE
template<class C>
void TEMP_DOMAIN::_destroyCPool(CPoolsT& cpools) noexcept {
	reinterpret_cast<ComponentPool<C, E>*>(cpools[staticTypedesc(C).wrap()].storage.data())->~ComponentPool();
}

TEMPLATE
template<class C>
ComponentPool<C, E>& TEMP_DOMAIN::_getCPool() noexcept {
	return *reinterpret_cast<ComponentPool<C, E>*>(_componentPools.find(staticTypedesc(C).wrap())->second.storage.data()
	);
}

TEMPLATE
template<class C>
const ComponentPool<C, E>& TEMP_DOMAIN::_getCPool() const noexcept {
	return *reinterpret_cast<const ComponentPool<C, E>*>(
		_componentPools.find(staticTypedesc(C).wrap())->second.storage.data()
	);
}

TEMPLATE
template<class C>
ComponentPool<C, E>* TEMP_DOMAIN::_tryGetCPool() noexcept {
	const auto found = _componentPools.find(staticTypedesc(C).wrap());
	return found != _componentPools.end() ? reinterpret_cast<ComponentPool<C, E>*>(found->second.storage.data()) :
											nullptr;
}

TEMPLATE template<class C>
const ComponentPool<C, E>* TEMP_DOMAIN::_tryGetCPool() const noexcept {
	const auto found = _componentPools.find(staticTypedesc(C).wrap());
	return found != _componentPools.end() ? reinterpret_cast<const ComponentPool<C, E>*>(found->second.storage.data()) :
											nullptr;
}

TEMPLATE
TEMP_DOMAIN::~Domain() noexcept {
	for (const auto& [type, destroyer] : _cpoolDestroyers) {
		destroyer(_componentPools);
	}
}

TEMPLATE
void TEMP_DOMAIN::swap(TEMP_DOMAIN& other) noexcept {
	std::swap(_entityPool, other._entityPool);
	std::swap(_componentPools, other._componentPools);
	std::swap(_cpoolDestroyers, other._cpoolDestroyers);
}

TEMPLATE
bool TEMP_DOMAIN::alive(const EntityT entity) const noexcept {
	return _entityPool.alive(entity);
}

TEMPLATE
bool TEMP_DOMAIN::contains(const IdT id) const noexcept {
	return _entityPool.contains(id);
}

TEMPLATE
TEMP_DOMAIN::VersionT TEMP_DOMAIN::version(const EntityT entity) const noexcept {
	return _entityPool.version(entity);
}

TEMPLATE
TEMP_DOMAIN::VersionT TEMP_DOMAIN::version(const IdT id) const noexcept {
	return _entityPool.version(id);
}

TEMPLATE
size_t TEMP_DOMAIN::count() const noexcept {
	return _entityPool.size();
}

TEMPLATE
TEMP_DOMAIN::EntityT TEMP_DOMAIN::newEntity() noexcept {
	return _entityPool.newEntity();
}

TEMPLATE
TEMP_DOMAIN::EntityT TEMP_DOMAIN::recycleEntity(const EntityT entity) noexcept {
	return _entityPool.recycleEntity(entity);
}

TEMPLATE
TEMP_DOMAIN::EntityT TEMP_DOMAIN::recycleId(const IdT id) noexcept {
	return _entityPool.recycleId(id);
}

TEMPLATE
void TEMP_DOMAIN::kill(const EntityT entity) noexcept {
	for (auto&& [type, poolStorage] : _componentPools) {
		reinterpret_cast<ErasableComponentPool<EntityT>*>(&poolStorage)->removeComponent(entity);
	}

	_entityPool.kill(entity);
}

TEMPLATE
void TEMP_DOMAIN::kill(std::input_iterator auto first, std::input_iterator auto last) noexcept {
	_entityPool.kill(first, last);
}

TEMPLATE
void TEMP_DOMAIN::kill(std::initializer_list<EntityT> entities) noexcept {
	_entityPool.kill(entities);
}

TEMPLATE
auto TEMP_DOMAIN::entities() const noexcept {
	return std::views::all(_entityPool);
}

TEMPLATE
template<class C, class... Args>
TEMP_DOMAIN::GetReference<C> TEMP_DOMAIN::addComponent(const EntityT entity, Args&&... args) noexcept {
	return _assureCPool<C>().addComponent(entity, std::forward<Args>(args)...);
}

TEMPLATE
template<class C>
TEMP_DOMAIN::GetReference<C> TEMP_DOMAIN::getComponent(const EntityT entity) noexcept {
	return _getCPool<C>().get(entity);
}

TEMPLATE
template<class C>
TEMP_DOMAIN::ConstGetReference<C> TEMP_DOMAIN::getComponent(const EntityT entity) const noexcept {
	return _getCPool<C>().get(entity);
}

TEMPLATE
template<class C>
std::optional<std::reference_wrapper<C>> TEMP_DOMAIN::tryGetComponent(const EntityT entity) noexcept
	requires(not _details::ComponentTraits<C, E>::flag)
{
	return _getCPool<C>().tryGet(entity);
}

TEMPLATE
template<class C>
std::optional<std::reference_wrapper<const C>> TEMP_DOMAIN::tryGetComponent(const EntityT entity) noexcept
	requires(not _details::ComponentTraits<C, E>::flag)
{
	return _getCPool<C>().tryGet(entity);
}

TEMPLATE
template<class C>
bool TEMP_DOMAIN::removeComponent(const EntityT entity) noexcept {
	return _assureCPool<C>().removeComponent(entity);
}

TEMPLATE
template<class C>
C TEMP_DOMAIN::removeComponent(const EntityT entity, MoveFlag) noexcept
	requires(std::movable<C> and not _details::ComponentTraits<C, E>::flag)
{
	return _assureCPool<C>().removeComponent(entity, moveFlag);
}

TEMPLATE
template<class C>
requires(not std::is_const_v<C>)
auto TEMP_DOMAIN::components() noexcept {
	return std::views::all(_assureCPool<C>());
}

TEMPLATE
template<class C>
requires(std::is_const_v<C>)
auto TEMP_DOMAIN::components() noexcept {
	return std::views::all(
		const_cast<const decltype(_assureCPool<std::remove_const_t<C>>())>(_assureCPool<std::remove_const_t<C>>())
	);
}

TEMPLATE
template<class C>
auto TEMP_DOMAIN::components() const noexcept {
	static const ComponentPool<C, E> empty;
	auto poolPtr = _tryGetCPool<C>();
	return poolPtr ? std::views::all(*poolPtr) : std::views::all(empty);
}

TEMPLATE
template<class C>
bool TEMP_DOMAIN::hasComponent(const EntityT entity) const noexcept {
	auto cpool = _tryGetCPool<C>();
	return cpool ? cpool->contains(entity) : false;
}

} // namespace arch::ecs

#undef TEMPLATE
#undef TEMP_DOMAIN
