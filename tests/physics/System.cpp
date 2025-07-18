#include <physics/RigidBodyComponent.h>
#include <Ecs.h>
#include <gtest/gtest.h>
#include <physics/System.h>
#include <Scene.h>

namespace physics {
namespace ecs = arch::ecs;
namespace phy = arch::physics;
namespace math = arch::math;
namespace scene = arch::scene;

TEST(Physics, TestUpdate) {
	ecs::Domain domain;
	phy::System system(domain);

	constexpr auto startV = math::float3(1.f, 0.f, 0.f);

	// Create movable entity
	auto e0 = domain.newEntity();
	auto& rigidBody = domain.addComponent<phy::RigidBodyComponent>(
		e0,
		{
			1.f,
			{1.f, 1.f, 0.f},
			startV
		}
	);
	auto& transform = domain.addComponent<scene::components::TransformComponent>(
		e0,
		{
			{0.f, 0.f, 0.f},
			{0.f, 0.f, 0.f, 1.f},
			math::float3(1)
		}
	);

	// Update physics system
	const math::f32 t = system.update();

	// Expect entity to move accordingly to the old velocity
	EXPECT_EQ(transform.position, t * startV);

	// Expect that velocity updated accordingly to the force
	EXPECT_EQ(rigidBody.velocity, startV + t * rigidBody.force);
}

TEST(Physics, CheckCollisions) {

}
} // namespace physics
