#ifndef ECS_GAME_COORDINATOR_H
#define ECS_GAME_COORDINATOR_H

#define template_base(T, B) template <typename T, std::enable_if_t<std::is_base_of_v<B, T>, bool> = true>

#include "component_manager.h"
#include "entity_manager.h"
#include "system_manager.h"
#include "component.h"

namespace arch::ecs {
	class GameCoordinator {
	public:
		void init();

		[[nodiscard]] EntityId create_entity() const;

		void destroy_entity(EntityId entity_id) const;

		template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
 		void add_component(EntityId entity_id, T component);

		template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
		void remove_component(EntityId entity_id);

		template<typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
		T &get_component(EntityId entityId) const;

		template<typename T, std::enable_if_t<std::is_base_of_v<System, T>, bool> = true>
		std::shared_ptr<T> add_system();

		template<typename T, std::enable_if_t<std::is_base_of_v<System, T>, bool> = true>
		void set_system_component_mask(ComponentMask mask);

	private:
		std::unique_ptr<ComponentManager> _componentManager;
		std::unique_ptr<EntityManager> _entityManager;
		std::unique_ptr<SystemManager> _systemManager;
	};

	template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
	void GameCoordinator::add_component(EntityId entity_id, T component) {
		_componentManager->add_component<T>(entity_id, component);

		auto mask = _entityManager->get_entity_mask(entity_id);
		mask.set(ComponentIdGenerator::get_component_id<T>(), true);
		_entityManager->set_entity_mask(entity_id, mask);
		_systemManager->on_entity_mask_change(entity_id, mask);
	}

	template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
	void GameCoordinator::remove_component(EntityId entity_id) {
		_componentManager->remove_component<T>(entity_id);

		auto mask = _entityManager->get_entity_mask(entity_id);
		mask.set(ComponentIdGenerator::get_component_id<T>(), false);
		_entityManager->set_entity_mask(entity_id, mask);
		_systemManager->on_entity_mask_change(entity_id, mask);
	}

	template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
	T &GameCoordinator::get_component(EntityId entityId) const {
		return _componentManager->get_component<T>(entityId);
	}

	template <typename T, std::enable_if_t<std::is_base_of_v<System, T>, bool>>
	std::shared_ptr<T> GameCoordinator::add_system() {
		auto s = _systemManager->add_system<T>();

		auto system = std::static_pointer_cast<System>(s);
		system->init();
		_entityManager->on_system_created(s);

		return s;
	}

	template <typename T, std::enable_if_t<std::is_base_of_v<System, T>, bool>>
	void GameCoordinator::set_system_component_mask(ComponentMask mask) {
		_systemManager->set_system_component_mask<T>(mask);
	}
}

#endif