//
// Created by tosiek on 30.12.22.
//

#include "transform.h"

std::pair<glm::vec4, glm::vec4> Transform::rotate(float angle, float x, float y, float z) {
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

    return std::make_pair(q, q1);
}

std::pair<glm::vec4, glm::vec4> Transform::rotate(float angle, glm::vec3 rotationAxis) {
    return rotate(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
}

glm::mat4 Transform::translate(glm::vec3 translationVector) {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translationVector);
    return translationMatrix;
}

glm::mat4 Transform::translate(float x, float y, float z) {
    glm::vec3 translationVector = glm::vec3(x, y, z);

    return translate(translationVector);
}
