#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/Shape.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {
using TransformComponent = arch::scene::components::TransformComponent;

///@brief Horizontal line collider, for map boundaries
///@warning Doesn't collide with other lines
struct HorizontalLine: public Shape {
	f32 y;

	HorizontalLine(f32 y);

	///@brief Used to cast the shape to the map coordinates
	f32 getRealPosition(const TransformComponent& transform) const;
};
} // namespace arch::physics
