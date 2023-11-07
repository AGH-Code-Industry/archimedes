#ifndef ECS_ENTITY_MANAGER_H
#define ECS_ENTITY_MANAGER_H

#include <queue>
#include <array>
#include <unordered_map>

#include "common.h"
#include "entity.h"
#include "system.h"

namespace arch::ecs {

class EntityManager {
public:
	EntityManager();

	EntityId create_entity();
	EntityId destroy_entity(EntityId entity_id);
	EntityId get_new_id();

	ComponentMask get_entity_mask(EntityId entity_id);
	void set_entity_mask(EntityId entity_id, ComponentMask mask);

	template <typename T>
	void on_system_created(T s);

private:
	std::queue<EntityId> _free_ids;
	std::vector<Entity> _entities;
	std::unordered_map<EntityId, uint32_t> _entity_id_to_index;

	uint32_t _entity_count;
};

template <typename T>
void EntityManager::on_system_created(T s) {
	auto system = std::static_pointer_cast<System>(s);

	for (int i = 0; i < _entity_count; i++) {
		if ((system->mask & _entities[i].mask) == system->mask) {
			system->entity_ids.insert(_entities[i].id);
		}
	}
}

}
#endif
