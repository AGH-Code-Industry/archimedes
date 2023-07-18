#include "ecs/systems/physics_system.h"
#include "ecs/game_coordinator.h"

using namespace arch::ecs;

void PhysicsSystem::init() {
    mask.set(ComponentIdGenerator::get_component_id<Transform>(), true);
    spdlog::info("PhysicsSystem initialized with mask: {}", mask.to_string());
}

void PhysicsSystem::update(float delta_time, const GameCoordinator &coordinator) {
    spdlog::info("PhysicsSystem update");
    for (auto entity_id : entity_ids) {
        auto& transform = coordinator.get_component<Transform>(entity_id);
		transform.position.x += delta_time;
		spdlog::info("Entity {} moved to position X: {}", entity_id, transform.position.x);
    }
}
