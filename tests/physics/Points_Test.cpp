#include <archimedes/physics/collisions/CollisionAlgorithms.hpp>

#include <archimedes/scene/components/TransformComponent.h>
#include <gtest/gtest.h>

namespace physics {

namespace phy = arch::physics;
namespace math = arch::math;
namespace scene = arch::scene;

using TransformComponent = scene::components::TransformComponent;

class PointContainmentTest: public ::testing::Test {};

TEST_F(PointContainmentTest, OBB_PointInside) {
	phy::OBB obb{
		{  0.f,  0.f },
		{ 0.5f, 0.5f },
		0.f
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(obb, t, { 0, 0 }));
}

TEST_F(PointContainmentTest, OBB_PointOutside) {
	phy::OBB obb{
		{  0.f,  0.f },
		{ 0.5f, 0.5f },
		0.f
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_FALSE(phy::checkPoint(obb, t, { 2, 2 }));
}

TEST_F(PointContainmentTest, OBB_PointOnEdge) {
	phy::OBB obb{
		{  0.f,  0.f },
		{ 0.5f, 0.5f },
		0.f
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(obb, t, { 0.5f, 0.0f }));
}

TEST_F(PointContainmentTest, OBB_Translated) {
	phy::OBB obb{
		{  0.f,  0.f },
		{ 0.5f, 0.5f },
		0.f
	};

	TransformComponent t{
		{ 2, 2, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(obb, t, { 2, 2 }));
	ASSERT_FALSE(phy::checkPoint(obb, t, { 0, 0 }));
}

TEST_F(PointContainmentTest, OBB_Rotated) {
	phy::OBB obb{
		{  0.f,  0.f },
		{ 0.5f, 0.5f },
		0.785398f
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(obb, t, { 0.2f, 0.0f }));
}

TEST_F(PointContainmentTest, OBB_NearEdgePrecision) {
	phy::OBB obb{
		{  0.f,  0.f },
		{ 0.5f, 0.5f },
		0.f
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(obb, t, { 0.5000001f, 0 }) || !phy::checkPoint(obb, t, { 0.5000001f, 0 }));
}

TEST_F(PointContainmentTest, Triangle_PointInside) {
	phy::Triangle tri{
		{  0.0f,	0.5f },
		{ -0.5f, -0.5f },
		{  0.5f, -0.5f }
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(tri, t, { 0, 0 }));
}

TEST_F(PointContainmentTest, Triangle_PointOutside) {
	phy::Triangle tri{
		{	  0,	 0.5f },
		{ -0.5f, -0.5f },
		{  0.5f, -0.5f }
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_FALSE(phy::checkPoint(tri, t, { 1, 1 }));
}

TEST_F(PointContainmentTest, Triangle_PointOnEdge) {
	phy::Triangle tri{
		{	  0,	 0.5f },
		{ -0.5f, -0.5f },
		{  0.5f, -0.5f }
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(tri, t, { 0, -0.5f }));
}

TEST_F(PointContainmentTest, Triangle_PointOnVertex) {
	phy::Triangle tri{
		{	  0,	 0.5f },
		{ -0.5f, -0.5f },
		{  0.5f, -0.5f }
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(tri, t, { 0, 0.5f }));
}

TEST_F(PointContainmentTest, Circle_PointInside) {
	phy::Circle circle{
		{ 0, 0 },
		1.0f
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(circle, t, { 0.5f, 0 }));
}

TEST_F(PointContainmentTest, Circle_PointOutside) {
	phy::Circle circle{
		{ 0, 0 },
		1.0f
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_FALSE(phy::checkPoint(circle, t, { 2, 0 }));
}

TEST_F(PointContainmentTest, Circle_PointOnEdge) {
	phy::Circle circle{
		{ 0, 0 },
		1.0f
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(circle, t, { 1.0f, 0 }));
}

TEST_F(PointContainmentTest, Circle_PrecisionEdge) {
	phy::Circle circle{
		{ 0, 0 },
		1.0f
	};

	TransformComponent t{
		{ 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(circle, t, { 0.9999999f, 0 }));
}

TEST_F(PointContainmentTest, Circle_Translated) {
	phy::Circle circle{
		{ 0, 0 },
		1.0f
	};

	TransformComponent t{
		{ 3, 3, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(circle, t, { 3.5f, 3 }));
	ASSERT_FALSE(phy::checkPoint(circle, t, { 0, 0 }));
}

TEST_F(PointContainmentTest, Triangle_Translated) {
	phy::Triangle tri{
		{  0.0f,	0.5f },
		{ -0.5f, -0.5f },
		{  0.5f, -0.5f }
	};

	TransformComponent t{
		{ 2, 2, 0 },
		{ 0, 0, 0, 1 },
		{ 1, 1, 1 }
	};

	ASSERT_TRUE(phy::checkPoint(tri, t, { 2, 2 }));
	ASSERT_FALSE(phy::checkPoint(tri, t, { 0, 0 }));
}

} // namespace physics
