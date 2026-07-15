#pragma once
#include <unordered_map>

#include <archimedes/ecs/Domain.h>
#include <archimedes/physics/collisions/Collision.h>

namespace arch::physics {

/// @brief Stores mouse-shape collisions in a unordered map
class MouseSet {
	std::unordered_map<ecs::Entity, CollisionState> _set;

public:

	///@brief Get all entities that collide with the mouse
	std::vector<ecs::Entity> getMouseEntities() const;

	///@brief CRUD operations
	void addCollision(ecs::Entity entity, CollisionState state);
	void updateCollision(ecs::Entity entity, CollisionState toState);
	void removeCollision(ecs::Entity entity);
	std::optional<CollisionState> getCollision(ecs::Entity entity) const;
};

} // namespace arch::physics
