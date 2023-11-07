#ifndef ECS_GAME_COORDINATOR_H
#define ECS_GAME_COORDINATOR_H

#include "component_manager.h"
#include "entity_manager.h"
#include "system_manager.h"
#include "component.h"


namespace arch::ecs {

template<typename T>
concept ComponentType = std::is_base_of_v<Component, T>;

template<typename T>
concept SystemType = std::is_base_of_v<System, T>;

class GameCoordinator {
public:
	GameCoordinator();

	EntityId create_entity() const;

	void destroy_entity(EntityId entity_id) const;

	template<ComponentType T>
	void add_component(EntityId entity_id, T component);

	template<ComponentType T>
	void remove_component(EntityId entity_id);

	template<ComponentType T>
	T &get_component(EntityId entityId) const;

	template<SystemType T>
	std::shared_ptr<T> add_system();

	template<SystemType T>
	void set_system_component_mask(ComponentMask mask);

private:
	std::unique_ptr<ComponentManager> _componentManager;
	std::unique_ptr<EntityManager> _entityManager;
	std::unique_ptr<SystemManager> _systemManager;
};

template<ComponentType T>
void GameCoordinator::add_component(EntityId entity_id, T component) {
	_componentManager->add_component<T>(entity_id, component);

	auto mask = _entityManager->get_entity_mask(entity_id);
	mask.set(ComponentIdGenerator::get_component_id<T>(), true);
	_entityManager->set_entity_mask(entity_id, mask);
	_systemManager->on_entity_mask_change(entity_id, mask);
}

template<ComponentType T>
void GameCoordinator::remove_component(EntityId entity_id) {
	_componentManager->remove_component<T>(entity_id);

	auto mask = _entityManager->get_entity_mask(entity_id);
	mask.set(ComponentIdGenerator::get_component_id<T>(), false);
	_entityManager->set_entity_mask(entity_id, mask);
	_systemManager->on_entity_mask_change(entity_id, mask);
}

template<ComponentType T>
T &GameCoordinator::get_component(EntityId entityId) const {
	return _componentManager->get_component<T>(entityId);
}

template<SystemType T>
std::shared_ptr<T> GameCoordinator::add_system() {
	auto s = _systemManager->add_system<T>();

	s->init();
	_entityManager->on_system_created(s);

	return s;
}

template<SystemType T>
void GameCoordinator::set_system_component_mask(ComponentMask mask) {
	_systemManager->set_system_component_mask<T>(mask);
}

}

#endif