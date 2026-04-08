#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/Shape.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {
using TransformComponent = arch::scene::components::TransformComponent;

///@brief Vertical line collider, used for map boundaries.
///@warning Can't collide with other lines
struct VerticalLine: public Shape {
	f32 x;

	VerticalLine(f32 x);

	///@brief Used to cast the shape to map coordinates
	f32 getRealPosition(const TransformComponent& transform) const;
};
} // namespace arch::physics
