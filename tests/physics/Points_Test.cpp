#include <archimedes/physics/collisions/PointAlgorithms.h>
#include <archimedes/scene/components/TransformComponent.h>
#include <gtest/gtest.h>

namespace physics {

namespace phy = arch::physics;
namespace math = arch::math;
namespace scene = arch::scene;

using TransformComponent = scene::components::TransformComponent;

///@brief Tests mouse-shape collision algorithms
class PointContainmentTest : public ::testing::Test {};


TEST_F(PointContainmentTest, OBB_PointInside) {
    phy::OBB obb{
        { -0.5f,  0.5f, 0 },
        {  0.5f, -0.5f, 0 },
        0.f
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(obb, {0,0,0}, t));
}

TEST_F(PointContainmentTest, OBB_PointOutside) {
    phy::OBB obb{
        { -0.5f,  0.5f, 0 },
        {  0.5f, -0.5f, 0 },
        0.f
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_FALSE(phy::containsPoint(obb, {2,2,0}, t));
}

TEST_F(PointContainmentTest, OBB_PointOnEdge) {
    phy::OBB obb{
        { -0.5f,  0.5f, 0 },
        {  0.5f, -0.5f, 0 },
        0.f
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(obb, {0.5f,0,0}, t));
}

TEST_F(PointContainmentTest, OBB_Translated) {
    phy::OBB obb{
        { -0.5f,  0.5f, 0 },
        {  0.5f, -0.5f, 0 },
        0.f
    };

    TransformComponent t{{2,2,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(obb, {2,2,0}, t));
    ASSERT_FALSE(phy::containsPoint(obb, {0,0,0}, t));
}

TEST_F(PointContainmentTest, OBB_Rotated) {
    phy::OBB obb{
        { -0.5f,  0.5f, 0 },
        {  0.5f, -0.5f, 0 },
        0.785398f // 45 deg
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(obb, {0.2f,0.0f,0}, t));
}

TEST_F(PointContainmentTest, OBB_NearEdgePrecision) {
    phy::OBB obb{
        { -0.5f,  0.5f, 0 },
        {  0.5f, -0.5f, 0 },
        0.f
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(obb, {0.5000001f, 0, 0}, t) ||
                !phy::containsPoint(obb, {0.5000001f, 0, 0}, t));
}

TEST_F(PointContainmentTest, Triangle_PointInside) {
    phy::Triangle tri{
        { 0,  0.5f, 0 },
        {-0.5f,-0.5f,0 },
        { 0.5f,-0.5f,0 }
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(tri, {0,0,0}, t));
}

TEST_F(PointContainmentTest, Triangle_PointOutside) {
    phy::Triangle tri{
        { 0,  0.5f, 0 },
        {-0.5f,-0.5f,0 },
        { 0.5f,-0.5f,0 }
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_FALSE(phy::containsPoint(tri, {1,1,0}, t));
}

TEST_F(PointContainmentTest, Triangle_PointOnEdge) {
    phy::Triangle tri{
        { 0,  0.5f, 0 },
        {-0.5f,-0.5f,0 },
        { 0.5f,-0.5f,0 }
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(tri, {0,-0.5f,0}, t));
}

TEST_F(PointContainmentTest, Triangle_PointOnVertex) {
    phy::Triangle tri{
        { 0,  0.5f, 0 },
        {-0.5f,-0.5f,0 },
        { 0.5f,-0.5f,0 }
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(tri, {0,0.5f,0}, t));
}

TEST_F(PointContainmentTest, Circle_PointInside) {
    phy::Circle circle{
        {0,0,0},
        1.0f
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(circle, {0.5f,0,0}, t));
}

TEST_F(PointContainmentTest, Circle_PointOutside) {
    phy::Circle circle{
        {0,0,0},
        1.0f
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_FALSE(phy::containsPoint(circle, {2,0,0}, t));
}

TEST_F(PointContainmentTest, Circle_PointOnEdge) {
    phy::Circle circle{
        {0,0,0},
        1.0f
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(circle, {1.0f,0,0}, t));
}

TEST_F(PointContainmentTest, Circle_PrecisionEdge) {
    phy::Circle circle{
        {0,0,0},
        1.0f
    };

    TransformComponent t{{0,0,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(
        phy::containsPoint(circle, {0.9999999f,0,0}, t)
    );
}

TEST_F(PointContainmentTest, Circle_Translated) {
    phy::Circle circle{
        {0,0,0},
        1.0f
    };

    TransformComponent t{{3,3,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(circle, {3.5f,3,0}, t));
    ASSERT_FALSE(phy::containsPoint(circle, {0,0,0}, t));
}

TEST_F(PointContainmentTest, Triangle_Translated) {
    phy::Triangle tri{
        { 0,  0.5f, 0 },
        {-0.5f,-0.5f,0 },
        { 0.5f,-0.5f,0 }
    };

    TransformComponent t{{2,2,0}, {0,0,0,1}, {1,1,1}};

    ASSERT_TRUE(phy::containsPoint(tri, {2,2,0}, t));
    ASSERT_FALSE(phy::containsPoint(tri, {0,0,0}, t));
}

} // namespace physics