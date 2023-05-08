#include <gtest/gtest.h>

#include "ecs/game_coordinator.h"
#include "ecs/components/transform.h"
#include "ecs/systems/physics_system.h"

namespace arch::ecs {
	TEST(ECSTest, MovingObjects) {

		GameCoordinator coordinator;
		coordinator.init();

		auto physics_system = coordinator.add_system<PhysicsSystem>();
		std::vector<EntityId> entity_ids;

	    for (int i = 0; i < 4; i++) {
			auto entity_id = coordinator.create_entity();
			coordinator.add_component<Transform>(entity_id, Transform{{0, 0, 0}, {0, 0, 0}, {1, 1, 1}});
			entity_ids.push_back(entity_id);
		}

		physics_system->update(1.0f, coordinator);

		for (auto i: entity_ids)
			EXPECT_FLOAT_EQ(coordinator.get_component<Transform>(i).position.x, 1.0f);

		auto new_entity_id = coordinator.create_entity();
		coordinator.add_component<Transform>(new_entity_id, Transform{{0, 0, 0}, {0, 0, 0}, {1, 1, 1}});
		physics_system->update(2.0f, coordinator);

		for (auto i : entity_ids)
			EXPECT_FLOAT_EQ(coordinator.get_component<Transform>(i).position.x, 3.0f);
		EXPECT_FLOAT_EQ(coordinator.get_component<Transform>(new_entity_id).position.x, 2.0f);
	}
}