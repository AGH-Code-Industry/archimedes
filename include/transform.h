//
// Created by tosiek on 30.12.22.
//

#include <glm/glm.hpp>
#include <cmath>
#include <utility>

#ifndef ARCHIMEDES_TRANSFORM_H
#define ARCHIMEDES_TRANSFORM_H


class Transform {

public:
    /**
     * Calculates quaternions needed for rotation relative to given vector components
     * @param angle rotation angle in degrees
     * @param x x component of the vector generating rotation axis
     * @param y y component of the vector generating rotation axis
     * @param z z component of the vector generating rotation axis
     * @return quaternions q and q^-1 which rotate point
     */
    static std::pair<glm::vec4, glm::vec4> rotate(float angle, float x, float y, float z);

    /**
     * Calculates quaternions needed for rotation relative to given vector components
     * @param angle rotation angle in degrees
     * @param rotationAxis vector generating rotation axis
     * @return quaternions q and q^-1 which rotate point
     */
    static std::pair<glm::vec4, glm::vec4> rotate(float angle, glm::vec3 rotationAxis);

    /**
     *
     * @param x
     * @param y
     * @param z
     * @return
     */
    glm::mat4 translate(float x, float y, float z);

    /**
     *
     * @param x
     * @param y
     * @param z
     * @return
     */
    glm::mat4 scale(float x, float y, float z);

    /**
     *
     * @param a
     * @return
     */
    glm::mat4 scale(float a);
};


#endif //ARCHIMEDES_TRANSFORM_H
