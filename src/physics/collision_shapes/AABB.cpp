#include <archimedes/physics/collision_shapes/AABB.h>

namespace arch::physics {
void AABB::update(float3 velocity, f32 time) {
	topLeft += velocity * time;
	bottomRight += velocity * time;
}
}