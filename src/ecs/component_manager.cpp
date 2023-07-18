#include "ecs/component_manager.h"

using namespace arch::ecs;

void ComponentManager::on_entity_destroyed(EntityId entity_id) {
    for (auto const& component_pool: _component_pools) {
		component_pool->on_entity_destroyed(entity_id);
    }
}
