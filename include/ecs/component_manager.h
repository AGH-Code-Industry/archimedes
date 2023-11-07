#ifndef ECS_COMPONENT_MANAGER_H
#define ECS_COMPONENT_MANAGER_H

#include <memory>
#include <vector>
#include "common.h"
#include "component_pool.h"

namespace arch::ecs {

class ComponentManager {
public:
	template<typename T>
	void add_component(EntityId entity_id, T component);

	template<typename T>
	void remove_component(EntityId entity_id);

	template<typename T>
	T& get_component(EntityId entity_id);

	void on_entity_destroyed(EntityId entity_id);

private:
	std::vector<std::shared_ptr<IComponentPool>> _component_pools;

	template<typename T>
	std::shared_ptr<ComponentPool<T>> _get_component_pool();
};

template<typename T>
void ComponentManager::add_component(EntityId entity_id, T component) {
	_get_component_pool<T>()->insert(entity_id, component);
}


template<typename T>
void ComponentManager::remove_component(EntityId entity_id) {
	_get_component_pool<T>()->remove(entity_id);
}

template<typename T>
T &ComponentManager::get_component(EntityId entity_id) {
	return _get_component_pool<T>()->get_data(entity_id);
}

template<typename T>
std::shared_ptr<ComponentPool<T>> ComponentManager::_get_component_pool() {
	ComponentId id = ComponentIdGenerator::get_component_id<T>();
	if (_component_pools.size() <= id || !_component_pools[id]) {
		// Register component if it doesn't exist
		_component_pools.emplace_back(std::make_shared<ComponentPool<T>>());
	}
	return std::static_pointer_cast<ComponentPool<T>>(_component_pools[id]);
}

}

#endif
