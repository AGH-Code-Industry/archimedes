#include <ranges>

#include <ecs/Domain.h>

namespace arch::ecs {

Domain::~Domain() noexcept {
	for (const auto& [type, destroyer] : _cpoolDestroyers) {
		destroyer(_componentPools);
	}
}

void Domain::swap(Domain& other) noexcept {
	std::swap(_entityPool, other._entityPool);
	std::swap(_componentPools, other._componentPools);
	std::swap(_cpoolDestroyers, other._cpoolDestroyers);
}

bool Domain::alive(const Entity entity) const noexcept {
	return _entityPool.contains(entity);
}

bool Domain::contains(const IdT id) const noexcept {
	return _entityPool.contains(id);
}

Domain::VersionT Domain::version(const Entity entity) const noexcept {
	return _entityPool.version(entity);
}

Domain::VersionT Domain::version(const IdT id) const noexcept {
	return _entityPool.version(id);
}

size_t Domain::count() const noexcept {
	return _entityPool.size();
}

Entity Domain::newEntity() noexcept {
	return _entityPool.newEntity();
}

Entity Domain::recycleEntity(const Entity entity) noexcept {
	return _entityPool.recycleEntity(entity);
}

Entity Domain::recycleId(const IdT id) noexcept {
	return _entityPool.recycleId(id);
}

void Domain::kill(const Entity entity) noexcept {
	for (auto&& [type, poolStorage] : _componentPools) {
		reinterpret_cast<_details::CommonComponentPool*>(&poolStorage)->removeComponent(entity);
	}

	_entityPool.kill(entity);
}

void Domain::kill(std::input_iterator auto first, std::input_iterator auto last) noexcept {
	_entityPool.kill(first, last);
}

void Domain::kill(std::initializer_list<Entity> entities) noexcept {
	_entityPool.kill(entities);
}

auto Domain::entities() const noexcept -> decltype(std::views::all(*std::declval<const EntityPool*>())) {
	return std::views::all(_entityPool);
}

Domain::Global::~Global() noexcept {
	deleter(ptr);

	ptr = {};
	deleter = {};
}

} // namespace arch::ecs
