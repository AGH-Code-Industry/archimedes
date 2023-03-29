#include "ecs/system_manager.h"

using namespace arch::ecs;

void SystemManager::on_entity_destroyed(EntityId entity_id) {
	for (auto const& system : _systems) {
		system->entity_ids.erase(entity_id);
	}
}

void SystemManager::on_entity_mask_change(EntityId entity_id,
										  ComponentMask entity_mask) {
	for (auto const& system : _systems) {
		auto& entity_ids = system->entity_ids;

		if ((entity_mask & system->mask) != system->mask) {
			entity_ids.erase(entity_id);
		} else {
			entity_ids.insert(entity_id);
		}
	}
}