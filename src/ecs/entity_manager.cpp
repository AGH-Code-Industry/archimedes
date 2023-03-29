#include "ecs/entity_manager.h"

using namespace arch::ecs;

EntityManager::EntityManager(): _entity_count(0) {}

EntityId EntityManager::create_entity() {
    assert_msg(_entity_count >= max_entities, "Maximum number of entity_ids reached.");

    EntityId id = get_new_id();
	_entities[id] = Entity(id, ComponentMask());
	_entity_count++;

    return id;
}

EntityId EntityManager::destroy_entity(EntityId entity_id) {
    assert_msg(entity_id >= max_entities, "Entity entity_id is out of range.");

	_entities[entity_id] = Entity();
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
    return _entities[entity_id].mask;
}

void EntityManager::set_entity_mask(EntityId entity_id, ComponentMask mask) {
	_entities[entity_id].mask = mask;
}


