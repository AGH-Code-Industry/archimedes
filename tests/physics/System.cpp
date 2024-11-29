#include <gtest/gtest.h>

#include <Ecs.h>
#include <physics/System.h>
#include <physics/components/Force.h>
#include <physics/components/Mass.h>
#include <physics/components/Velocity.h>

TEST(Physics, TestUpdate) {
	namespace ecs = arch::ecs;
	namespace phy = arch::physics;
	namespace math = arch::math;
	ecs::Domain domain;

	auto e0 = domain.newEntity();

	domain.addComponent<phy::Force>(e0);
	domain.addComponent<phy::Mass>(e0);
	domain.addComponent<phy::Velocity>(e0);

	domain.getComponent<phy::Force>(e0).value = math::float2(1.0f, 1.0f);
	domain.getComponent<phy::Mass>(e0).mass = 1.0f;

	phy::System::setDomain(domain);

	phy::System::update();

	EXPECT_EQ(domain.getComponent<phy::Velocity>(e0).value, math::float2(1.0f, 1.0f));
}
