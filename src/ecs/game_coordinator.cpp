#include "ecs/game_coordinator.h"

using namespace arch::ecs;

void GameCoordinator::init() {
    _componentManager = std::make_unique<ComponentManager>();
    _entityManager = std::make_unique<EntityManager>();
    _systemManager = std::make_unique<SystemManager>();
}

EntityId GameCoordinator::create_entity() const {
    auto entity_id = _entityManager->create_entity();
    return entity_id;
}

void GameCoordinator::destroy_entity(EntityId entity_id) const {
	_entityManager->destroy_entity(entity_id);
	_componentManager->on_entity_destroyed(entity_id);
	_systemManager->on_entity_destroyed(entity_id);
}

