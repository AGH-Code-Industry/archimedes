//
// Created by tosiek on 30.12.22.
//

#include "transform.h"

using namespace arch;

std::pair<Quaternion, Quaternion> Transform::rotate(float angle, float x, float y, float z) {
    //normalize x,y,z
    auto norm =  static_cast<float>(sqrt(pow(x,2) + pow(y,2) + pow(z,2)));
    x /= norm;
    y /= norm;
    z /= norm;

    //fix angle
    angle /= 2;
    const double pi = std::acos(-1);
    angle *= static_cast<float>(pi/180.0);

    //create rotation quaternions
    float a = x * std::sin(angle);
    float b = y * std::sin(angle);
    float c = z * std::sin(angle);
    glm::vec3 imaginaryPart = glm::vec3(a,b,c);
    glm::vec4 q = glm::vec4(imaginaryPart, std::cos(angle));
    glm::vec4 q1 = glm::vec4(-imaginaryPart, std::cos(angle));

    return {q, q1};
}

std::pair<Quaternion, Quaternion> Transform::rotate(float angle, glm::vec3 rotation_axis) {
    return rotate(angle, rotation_axis.x, rotation_axis.y, rotation_axis.z);
}

glm::mat4 Transform::translate(glm::vec3 translation_vector) {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation_vector);
    return translationMatrix;
}

glm::mat4 Transform::translate(float x, float y, float z) {
    glm::vec3 translationVector = glm::vec3(x, y, z);

    return translate(translationVector);
}

glm::mat4 Transform::scale(glm::vec3 scaling_vector) {
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scaling_vector);

    return scalingMatrix;
}

glm::mat4 Transform::scale(float x, float y, float z) {
    glm::vec3 scalingVector = glm::vec3(x, y, z);

    return scale(scalingVector);
}

glm::mat4 Transform::scale(float a) {
    auto scalingVector = glm::vec3(a);

    return scale(scalingVector);
}
