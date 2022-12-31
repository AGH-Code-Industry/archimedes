//
// Created by tosiek on 31.12.22.
//

#include <gtest/gtest.h>
#include <transform.h>

TEST(TransformTest, RotationQuaternions) {
    glm::vec3 rotationAxis = glm::vec3(1, 1, 1);
    float rotationAngle = 120.0f;
    auto quaternions = Transform::rotate(rotationAngle, rotationAxis);

    EXPECT_FLOAT_EQ(quaternions.first.x, 0.5f);
    EXPECT_FLOAT_EQ(quaternions.first.y, 0.5f);
    EXPECT_FLOAT_EQ(quaternions.first.z, 0.5f);
    EXPECT_FLOAT_EQ(quaternions.first.w, 0.5f);

    EXPECT_FLOAT_EQ(quaternions.second.x, -0.5f);
    EXPECT_FLOAT_EQ(quaternions.second.y, -0.5f);
    EXPECT_FLOAT_EQ(quaternions.second.z, -0.5f);
    EXPECT_FLOAT_EQ(quaternions.second.w, 0.5f);
}

TEST(TransformTest, RotatatedPoint) {
    glm::vec3 rotationAxis = glm::vec3(1, 1, 1);
    float rotationAngle = 120.0f;
    auto quaternions = Transform::rotate(rotationAngle, rotationAxis);
    glm::vec4 point = glm::vec4(1, 2, 3, 0);

    auto multiplyQuaternions = [](glm::vec4 q1, glm::vec4 q2){
        float a = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
        float i = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
        float j = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
        float k = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;

        return glm::vec4(i,j,k,a);
    };

    glm::vec4 pPrim = multiplyQuaternions(quaternions.first,point);
    pPrim = multiplyQuaternions(pPrim, quaternions.second);

    EXPECT_FLOAT_EQ(pPrim.x, 3);
    EXPECT_FLOAT_EQ(pPrim.y, 1);
    EXPECT_FLOAT_EQ(pPrim.z, 2);
}