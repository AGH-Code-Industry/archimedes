#ifndef ECS_SYSTEM_MANAGER_H
#define ECS_SYSTEM_MANAGER_H

#include <vector>
#include "common.h"
#include "system.h"

namespace arch::ecs {
	class SystemManager {
	public:
		template <typename T>
		std::shared_ptr<T> add_system();

		template <typename T>
		void set_system_component_mask(ComponentMask mask);

		void on_entity_destroyed(EntityId entity_id);
		void on_entity_mask_change(EntityId entity_id, ComponentMask entity_mask);

	private:
		std::vector<std::shared_ptr<System>> _systems;
	};

	template <typename T>
	std::shared_ptr<T> SystemManager::add_system() {
		SystemId id = SystemIdGenerator::get_system_id<T>();
		assert_msg(id < _systems.size(), "System already registered. " +
										std::to_string(id) + " " +
										std::to_string(_systems.size()));

		// If system doesn't exist, create it and apply mask
		auto system = std::make_shared<T>();
		_systems.push_back(system);

		return system;
	}

	template <typename T>
	void SystemManager::set_system_component_mask(ComponentMask mask) {
		SystemId id = SystemIdGenerator::get_system_id<T>();
		assert_msg(id >= _systems.size(), "System not registered.");

		// If system exists, set mask
		_systems[id]->mask = mask;
	}
}


#endif
