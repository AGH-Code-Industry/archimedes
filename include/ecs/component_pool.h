#ifndef ECS_COMPONENT_POOL_H
#define ECS_COMPONENT_POOL_H

#include <vector>
#include <unordered_map>

#include "common.h"

namespace arch::ecs {

class IComponentPool {
public:
	virtual ~IComponentPool() = default;
	virtual void on_entity_destroyed(EntityId entity_id) = 0;
};

template <typename T>
class ComponentPool : public IComponentPool {
public:
	void insert(EntityId entity_id, T component);
	void remove(EntityId entity_id);

	T& get_data(EntityId entity_id);
	void on_entity_destroyed(EntityId entity_id) override;

private:
	std::vector<T> _component_array;
	std::unordered_map<EntityId, size_t> _entity_to_index_map;
	std::unordered_map<size_t, EntityId> _index_to_entity_map;
};

template <typename T>
void ComponentPool<T>::insert(EntityId entity_id, T component) {
	if (_entity_to_index_map.contains(entity_id)) {
		_component_array[_entity_to_index_map[entity_id]] = component;
		return;
	}

	size_t new_index = _component_array.size();
	_entity_to_index_map[entity_id] = new_index;
	_index_to_entity_map[new_index] = entity_id;
	_component_array.push_back(component);
}

template <typename T>
void ComponentPool<T>::remove(EntityId entity_id) {
	assert_msg(!_entity_to_index_map.contains(entity_id),
			   "Entity does not have component of this type.")

	size_t index_of_removed_entity = _entity_to_index_map[entity_id];
	size_t index_of_last_element = _component_array.size() - 1;
	_component_array[index_of_removed_entity] =
		_component_array[index_of_last_element];

	EntityId entity_of_last_element = _index_to_entity_map[index_of_last_element];
	_entity_to_index_map[entity_of_last_element] = index_of_removed_entity;
	_index_to_entity_map[index_of_removed_entity] = entity_of_last_element;

	_entity_to_index_map.erase(entity_id);
	_index_to_entity_map.erase(index_of_last_element);

	_component_array.pop_back();
}

template <typename T>
T& ComponentPool<T>::get_data(EntityId entity_id) {
	assert_msg(!_entity_to_index_map.contains(entity_id),
			   "Entity does not have component of this type.");
	return _component_array[_entity_to_index_map[entity_id]];
}

template <typename T>
void ComponentPool<T>::on_entity_destroyed(EntityId entity_id) {
	remove(entity_id);
}

}

#endif