#include <gtest/gtest.h>

#include <Ecs.h>
#include <physics/System.h>
#include <physics/components/Moveable.h>

TEST(Physics, TestUpdate) {
	namespace ecs = arch::ecs;
	namespace phy = arch::physics;
	namespace math = arch::math;

	ecs::Domain domain;
	phy::System system(domain);

	auto e0 = domain.newEntity();

	// Create movable entity
	phy::Moveable& entity = domain.addComponent<phy::Moveable>(e0);
	entity.center = {.mass = 1.f, .position = math::float2(0.f, 0.f)};
	constexpr auto startV = math::float2(1.f, 0.f);
	entity.velocity = startV;

	// Add force to it
	entity.force = math::float2(1.f, 1.f);

	// Update physics system
	const math::f32 t = system.update();

	// Expect entity to move accordingly to the old velocity
	EXPECT_EQ(entity.center.position, t * startV);

	// Expect that velocity updated accordingly to the force
	EXPECT_EQ(entity.velocity, startV + t * entity.force);
}
