#include <archimedes/Scene.h>
#include <archimedes/physics/PhysicsSystem.h>
#include <archimedes/physics/collisions/ColliderComponent.h>
#include <archimedes/physics/components/RigidBodyComponent.h>
#include <gtest/gtest.h>

namespace physics {
namespace ecs = arch::ecs;
namespace phy = arch::physics;
namespace math = arch::math;
namespace scene = arch::scene;

using TransformComponent = scene::components::TransformComponent;

void expectNormalApprox(const math::float3& actual, const math::float3& expected, float eps = 1e-4f) {
	bool same = std::abs(actual.x - expected.x) < eps && std::abs(actual.y - expected.y) < eps &&
		std::abs(actual.z - expected.z) < eps;

	bool opposite = std::abs(actual.x + expected.x) < eps && std::abs(actual.y + expected.y) < eps &&
		std::abs(actual.z + expected.z) < eps;

	EXPECT_TRUE(same || opposite);
}

class CollisionTest: public ::testing::Test {
protected:
	void SetUp() override {
		_domain = std::make_unique<ecs::Domain>();
		_system = std::make_unique<phy::PhysicsSystem>(*_domain, 800.f, 600.f);
	}

	ecs::Entity createOBB(
		const math::float3& pos,
		const math::Quat& rot,
		const math::float3& scale,
		const phy::OBB& obb
	) {
		auto e = _domain->newEntity();
		_domain->addComponent<TransformComponent>(e, { pos, rot, scale });
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = obb });
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
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = tri });
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
			->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = circle});
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
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = line });
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
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = line });
		return e;
	}

	std::unique_ptr<ecs::Domain> _domain;
	std::unique_ptr<phy::PhysicsSystem> _system;
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

TEST_F(CollisionTest, Mask_NoMatch) {
    phy::Circle circle{{0,0,0}, 1.0f};

    auto e1 = createCircle({0,0,0}, {0,0,0,1}, {1,1,1}, circle);
    auto e2 = createCircle({0.5f,0,0}, {0,0,0,1}, {1,1,1}, circle);

    auto& c1 = _domain->getComponent<phy::ColliderComponent>(e1);
    auto& c2 = _domain->getComponent<phy::ColliderComponent>(e2);

    c1.isScannedMask = std::bitset<32>(1 << 0);
    c1.scansMask     = std::bitset<32>(1 << 0);

    c2.isScannedMask = std::bitset<32>(1 << 1);
    c2.scansMask     = std::bitset<32>(1 << 1);

    ASSERT_FALSE(phy::ColliderComponent::areColliding(
        c1, c2,
        _domain->getComponent<TransformComponent>(e1),
        _domain->getComponent<TransformComponent>(e2)
    ));
}

TEST_F(CollisionTest, Mask_OneWayMatch) {
    phy::Circle circle{{0,0,0}, 1.0f};

    auto e1 = createCircle({0,0,0}, {0,0,0,1}, {1,1,1}, circle);
    auto e2 = createCircle({0.5f,0,0}, {0,0,0,1}, {1,1,1}, circle);

    auto& c1 = _domain->getComponent<phy::ColliderComponent>(e1);
    auto& c2 = _domain->getComponent<phy::ColliderComponent>(e2);

    c1.isScannedMask = std::bitset<32>(0);
    c1.scansMask     = std::bitset<32>(1 << 1);
	
    c2.isScannedMask = std::bitset<32>(1 << 1);
    c2.scansMask     = std::bitset<32>(0);

    ASSERT_TRUE(phy::ColliderComponent::areColliding(
        c1, c2,
        _domain->getComponent<TransformComponent>(e1),
        _domain->getComponent<TransformComponent>(e2)
    ));

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
        c2, c1,
		_domain->getComponent<TransformComponent>(e2),
        _domain->getComponent<TransformComponent>(e1)
    ));
}

TEST_F(CollisionTest, Mask_MultipleLayers) {
    phy::Circle circle{{0,0,0}, 1.0f};

    auto e1 = createCircle({0,0,0}, {0,0,0,1}, {1,1,1}, circle);
    auto e2 = createCircle({0.5f,0,0}, {0,0,0,1}, {1,1,1}, circle);

    auto& c1 = _domain->getComponent<phy::ColliderComponent>(e1);
    auto& c2 = _domain->getComponent<phy::ColliderComponent>(e2);

    c1.isScannedMask = std::bitset<32>(0);
    c1.scansMask     = std::bitset<32>(3);

    c2.isScannedMask = std::bitset<32>(1);
    c2.scansMask     = std::bitset<32>(0);

    ASSERT_TRUE(phy::ColliderComponent::areColliding(
        c1, c2,
        _domain->getComponent<TransformComponent>(e1),
        _domain->getComponent<TransformComponent>(e2)
    ));
}


TEST_F(CollisionTest, CircleVsCircle_Collision_Data) {
	phy::Circle circle{
		{ 0, 0, 0 },
		0.5f
	};

	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 0.8f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto result = phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	);

	ASSERT_TRUE(result);

	const auto& col = result.value();

	EXPECT_NEAR(col.depth, 0.2f, 1e-4f);

	expectNormalApprox(col.normal, { 1.f, 0.f, 0.f });
}

TEST_F(CollisionTest, OBBvsOBB_Collision_Data) {
	phy::OBB obb{
		{ -0.5f,	 0.5f, 0.f },
		{  0.5f, -0.5f, 0.f },
		0.f
	};

	auto e1 = createOBB({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);
	auto e2 = createOBB({ 0.8f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	auto result = phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	);

	ASSERT_TRUE(result);

	const auto& col = result.value();

	EXPECT_NEAR(col.depth, 0.2f, 1e-4f);

	expectNormalApprox(col.normal, { 1.f, 0.f, 0.f });
}

TEST_F(CollisionTest, HorizontalLineVsCircle_DepthNormal) {
	phy::HorizontalLine line{ 0.0f };
	phy::Circle circle{
		{ 0, 0, 0 },
		0.5f
	};

	auto e1 = createHorizontalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createCircle({ 0, 0.3f, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto colOpt = phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	);

	ASSERT_TRUE(colOpt.has_value());
	auto col = colOpt.value();

	EXPECT_NEAR(col.depth, 0.2f, 1e-4f);

	EXPECT_TRUE(
		(std::abs(col.normal.y - 1.0f) < 1e-4f && std::abs(col.normal.x) < 1e-4f) ||
		(std::abs(col.normal.y + 1.0f) < 1e-4f && std::abs(col.normal.x) < 1e-4f)
	);
}

TEST_F(CollisionTest, VerticalLineVsCircle_DepthNormal) {
	phy::VerticalLine line{ 0.0f };
	phy::Circle circle{
		{ 0, 0, 0 },
		0.5f
	};

	auto e1 = createVerticalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createCircle({ 0.3f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto colOpt = phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	);

	ASSERT_TRUE(colOpt.has_value());
	auto col = colOpt.value();

	EXPECT_NEAR(col.depth, 0.2f, 1e-4f);

	EXPECT_TRUE(
		(std::abs(col.normal.x - 1.0f) < 1e-4f && std::abs(col.normal.y) < 1e-4f) ||
		(std::abs(col.normal.x + 1.0f) < 1e-4f && std::abs(col.normal.y) < 1e-4f)
	);
}

TEST_F(CollisionTest, TriangleVsHorizontalLine_DepthNormal) {
	phy::HorizontalLine line{ 0.0f };

	phy::Triangle tri{
		{  0,  0.4f, 0 },
		{ -0.5f, -0.2f, 0 },
		{  0.5f, -0.2f, 0 }
	};

	auto e1 = createHorizontalLine({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, line);
	auto e2 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	auto colOpt = phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	);

	ASSERT_TRUE(colOpt.has_value());
	const auto& col = colOpt.value();

	EXPECT_NEAR(col.depth, 0.2f, 1e-4f);

	EXPECT_TRUE(
		(std::abs(col.normal.y - 1.0f) < 1e-4f && std::abs(col.normal.x) < 1e-4f) ||
		(std::abs(col.normal.y + 1.0f) < 1e-4f && std::abs(col.normal.x) < 1e-4f)
	);
}


} // namespace physics
