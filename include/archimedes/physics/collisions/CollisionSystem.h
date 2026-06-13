#pragma once
#include <archimedes/ecs/Domain.h>
#include <archimedes/physics/collisions/CollisionGraph.h>
#include <archimedes/physics/collisions/MouseSet.h>

namespace arch::physics {

///@brief Calculates and stores all collisions
class CollisionSystem {
	ecs::Domain& _domain;

	/// @brief Current shape-shape collision state
	CollisionGraph _savedCollisions;

	/// @brief Current mouse-shape collision state
	MouseSet _mouseCollisions;

	/// @brief Calculate current shape-shape collisions
	CollisionGraph _getCollidedEntities() const;

	/// @brief Check which stored shape-shape collisions didn't appear in current frame, update the graph
	void _checkDisappearedCollisions(const CollisionGraph& newCollisions);

	/// @brief Read shape-shape collisions that appeared in current frame, update the graph
	void _readCurrentCollisions(const CollisionGraph& newCollisions);

	/// @brief Calculate current mouse-shape collisions
	MouseSet _getMouseEntities(float2 mousePosition) const;

	/// @brief Check which stored mouse-shape collisions didn't appear in current frame, update the graph
	void _checkDisappearedMouseCollisions(const MouseSet& newMouseCollisions);

	/// @brief Read mouse-shape collisions that appeared in current frame, update the graph
	void _readCurrentMouseCollisions(const MouseSet& newMouseCollisions);

public:
	explicit CollisionSystem(ecs::Domain& domain);

	/// @brief Get shape-shape collisions that appeared in current frame
	std::unordered_map<ecs::Entity, Collision> getEnteredCollisions(ecs::Entity entity) const;

	/// @brief Get shape-shape collisions (all of them)
	std::unordered_map<ecs::Entity, Collision> getCollisions(ecs::Entity entity) const;

	/// @brief Get shape-shape collisions that disappeared in current frame
	std::unordered_map<ecs::Entity, Collision> getExitedCollisions(ecs::Entity entity) const;

	/// @brief Check if mouse collided with the shape in current frame
	bool hasMouseEntered(ecs::Entity entity) const;

	/// @brief Check if mouse collides with the shape (doesn't matter for how long)
	bool hasMouse(ecs::Entity entity) const;

	/// @brief Check if mouse stopped colliding with the shape in current frame
	bool hasMouseExited(ecs::Entity entity) const;

	/// @brief Calculate the collisions and update the graph, use it each frame
	void update(float2 mousePosition);
};
} // namespace arch::physics
