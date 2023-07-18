#include "ecs/entity_manager.h"

using namespace arch::ecs;

EntityManager::EntityManager(): _entity_count(0) {
	_entities.reserve(initial_entities);
}

EntityId EntityManager::create_entity() {
    EntityId id = get_new_id();
	_entity_id_to_index[id] = _entity_count;
	_entities.push_back(Entity(id, ComponentMask()));
	_entity_count++;

    return id;
}

EntityId EntityManager::destroy_entity(EntityId entity_id) {
	assert_msg(_entity_id_to_index.find(entity_id) == _entity_id_to_index.end(), "Entity doesn't exist.");

	// TODO: Border case when entity is the last one in the vector
	_entities[_entity_id_to_index[entity_id]] = _entities[_entity_count - 1];
	_entity_id_to_index[_entities[_entity_count - 1].id] = _entity_id_to_index[entity_id];
	_free_ids.push(entity_id);
	_entity_count--;

    return entity_id;
}

EntityId EntityManager::get_new_id() {
    if (!_free_ids.empty()) {
        EntityId id = _free_ids.front();
		_free_ids.pop();
        return id;
    }
    return _entity_count + 1;
}

ComponentMask EntityManager::get_entity_mask(EntityId entity_id) {
    return _entities[_entity_id_to_index[entity_id]].mask;
}

void EntityManager::set_entity_mask(EntityId entity_id, ComponentMask mask) {
	_entities[_entity_id_to_index[entity_id]].mask = mask;
}


