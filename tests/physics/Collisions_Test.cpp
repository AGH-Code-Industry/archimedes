#include <archimedes/Scene.h>
#include <archimedes/physics/PhysicsSystem.h>
#include <archimedes/physics/components/ColliderComponent.h>
#include <archimedes/physics/components/RigidBodyComponent.h>
#include <gtest/gtest.h>

namespace physics {
namespace ecs = arch::ecs;
namespace phy = arch::physics;
namespace math = arch::math;
namespace scene = arch::scene;

using TransformComponent = scene::components::TransformComponent;

class CollisionTest: public ::testing::Test {
protected:
	void SetUp() override {
		_domain = std::make_unique<ecs::Domain>();
		_system = std::make_unique<phy::PhysicsSystem>(*_domain);
		_collision = [&, &domain = _domain](ecs::Entity, ecs::Entity) {
		};
	}

	ecs::Entity createOBB(
		const math::float3& pos,
		const math::Quat& rot,
		const math::float3& scale,
		const phy::OBB& obb
	) {
		auto e = _domain->newEntity();
		_domain->addComponent<TransformComponent>(e, { pos, rot, scale });
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = obb, .action = _collision });
		return e;
	}

	ecs::Entity createTriangle(
		const math::float3& pos,
		const math::Quat& rot,
		const math::float3& scale,
		const phy::Triangle& tri
	) {
		auto e = _domain->newEntity();
		_domain->addComponent<TransformComponent>(e, { pos, rot, scale });
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = tri, .action = _collision });
		return e;
	}

	ecs::Entity createCircle(
		const math::float3& pos,
		const math::Quat& rot,
		const math::float3& scale,
		const phy::Circle& circle
	) {
		auto e = _domain->newEntity();
		_domain->addComponent<TransformComponent>(e, { pos, rot, scale });
		_domain
			->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = circle, .action = _collision });
		return e;
	}

	ecs::Entity createHorizontalLine(
		const math::float3& pos,
		const math::Quat& rot,
		const math::float3& scale,
		const phy::HorizontalLine& line
	) {
		auto e = _domain->newEntity();
		_domain->addComponent<TransformComponent>(e, { pos, rot, scale });
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = line, .action = _collision });
		return e;
	}

	ecs::Entity createVerticalLine(
		const math::float3& pos,
		const math::Quat& rot,
		const math::float3& scale,
		const phy::VerticalLine& line
	) {
		auto e = _domain->newEntity();
		_domain->addComponent<TransformComponent>(e, { pos, rot, scale });
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = line, .action = _collision });
		return e;
	}

	std::unique_ptr<ecs::Domain> _domain;
	std::unique_ptr<phy::PhysicsSystem> _system;
	std::function<void(ecs::Entity, ecs::Entity)> _collision;
};

TEST_F(CollisionTest, OBBvsOBB_Collision) {
	phy::OBB obb{
		{ -0.25f,  0.25f, 0.f },
		{  0.25f, -0.25f, 0.f },
		0.f
	};
	auto e1 = createOBB({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);
	auto e2 = createOBB({ 0.2f, 0.2f, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, OBBvsOBB_TouchingEdges) {
	phy::OBB obb{
		{ -0.25f,  0.25f, 0.f },
		{  0.25f, -0.25f, 0.f },
		0.f
	};
	auto e1 = createOBB({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);
	auto e2 = createOBB({ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, OBBvsOBB_RotatedCollision) {
	phy::OBB obb1{
		{ -0.25f,  0.25f, 0 },
		{  0.25f, -0.25f, 0 },
		0.f
	};
	phy::OBB obb2{
		{ -0.25f,  0.25f, 0 },
		{  0.25f, -0.25f, 0 },
		0.785398f
	}; // ~45 deg

	auto e1 = createOBB({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb1);
	auto e2 = createOBB({ 0.3f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb2);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, CircleVsCircle_Collision) {
	phy::Circle circle{
		{ 0, 0, 0 },
		0.4f
	};
	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, CircleVsCircle_NoCollision) {
	phy::Circle circle{
		{ 0, 0, 0 },
		0.4f
	};
	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 1.0f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, TriangleVsTriangle_NoCollision) {
	phy::Triangle tri{
		{	  0,	 0.5f, 0 },
		{ -0.5f, -0.5f, 0 },
		{  0.5f, -0.5f, 0 }
	};
	auto e1 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);
	auto e2 = createTriangle({ 2, 2, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, TriangleVsTriangle_Collision) {
	phy::Triangle tri{
		{	  0,	 0.5f, 0 },
		{ -0.5f, -0.5f, 0 },
		{  0.5f, -0.5f, 0 }
	};
	auto e1 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);
	auto e2 = createTriangle({ 0.2f, 0.1f, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, TriangleVsTriangle_Touching) {
	phy::Triangle tri{
		{	  0,	 0.5f, 0 },
		{ -0.5f, -0.5f, 0 },
		{  0.5f, -0.5f, 0 }
	};
	auto e1 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);
	auto e2 = createTriangle({ 1, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, TriangleVsCircle_NoCollision) {
	phy::Triangle tri{
		{	  0,	 0.5f, 0 },
		{ -0.5f, -0.5f, 0 },
		{  0.5f, -0.5f, 0 }
	};
	phy::Circle circle{
		{ 0, 0, 0 },
		0.3f
	};

	auto e1 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);
	auto e2 = createCircle({ 2, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, TriangleVsOBB_Collision) {
	phy::Triangle tri{
		{	  0,	 0.5f, 0 },
		{ -0.5f, -0.5f, 0 },
		{  0.5f, -0.5f, 0 }
	};
	phy::OBB obb{
		{ -0.25f,  0.25f, 0 },
		{  0.25f, -0.25f, 0 },
		0.f
	};

	auto e1 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);
	auto e2 = createOBB({ 0.2f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, HorizontalLineVsCircle_Collision) {
	phy::HorizontalLine line{ 0.0f };
	phy::Circle circle{
		{ 0, 0, 0 },
		0.5f
	};

	auto e1 = createHorizontalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createCircle({ 0, 0.3f, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, HorizontalLineVsCircle_NoCollision) {
	phy::HorizontalLine line{ 0.0f };
	phy::Circle circle{
		{ 0, 0, 0 },
		0.3f
	};

	auto e1 = createHorizontalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createCircle({ 0, 1.0f, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, HorizontalLineVsTriangle_Crossing) {
	phy::HorizontalLine line{ 0.0f };
	phy::Triangle tri{
		{  0,	 1, 0 },
		{ -1, -1, 0 },
		{  1, -1, 0 }
	};

	auto e1 = createHorizontalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, HorizontalLineVsTriangle_Above) {
	phy::HorizontalLine line{ 0.0f };
	phy::Triangle tri{
		{  0, 2, 0 },
		{ -1, 1, 0 },
		{  1, 1, 0 }
	};

	auto e1 = createHorizontalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, VerticalLineVsTriangle_Crossing) {
	phy::VerticalLine line{ 0.0f };
	phy::Triangle tri{
		{  1,	 0, 0 },
		{ -1, -1, 0 },
		{ -1,  1, 0 }
	};

	auto e1 = createVerticalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, VerticalLineVsTriangle_NoCollision) {
	phy::VerticalLine line{ 0.0f };
	phy::Triangle tri{
		{ 2,	 0, 0 },
		{ 3,	 1, 0 },
		{ 3, -1, 0 }
	};

	auto e1 = createVerticalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, HorizontalLineVsOBB_Crossing) {
	phy::HorizontalLine line{ 0.0f };
	phy::OBB obb{
		{ -0.5f,	 0.5f, 0 },
		{  0.5f, -0.5f, 0 },
		0.f
	};

	auto e1 = createHorizontalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createOBB({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, VerticalLineVsCircle_Touching) {
	phy::VerticalLine line{ 0.0f };
	phy::Circle circle{
		{ 0, 0, 0 },
		1.0f
	};

	auto e1 = createVerticalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createCircle({ 1.0f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

} // namespace physics
