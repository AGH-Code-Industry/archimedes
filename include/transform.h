//
// Created by tosiek on 30.12.22.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <utility>

#ifndef ARCHIMEDES_TRANSFORM_H
#define ARCHIMEDES_TRANSFORM_H

namespace arch {
    class Transform {

    public:
        /**
         * Calculates quaternions needed for rotation relative to given vector components
         * @param angle rotation angle in degrees
         * @param x x component of the vector generating rotation axis
         * @param y y component of the vector generating rotation axis
         * @param z z component of the vector generating rotation axis
         * @return std::pair<glm::vec4, glm::vec4> - (q, q^-1)
         */
        static std::pair<glm::vec4, glm::vec4> rotate(float angle, float x, float y, float z);

        /**
         * Calculates quaternions needed for rotation relative to given vector components
         * @param angle rotation angle in degrees
         * @param rotationAxis vector generating rotation axis
         * @return std::pair<glm::vec4, glm::vec4> - (q, q^-1)
         */
        static std::pair<glm::vec4, glm::vec4> rotate(float angle, glm::vec3 rotationAxis);

        /**
         * Creates matrix needed to shift point by given vector.
         * @param translationVector vector used to translate point
         * @return glm::mat4 needed for translating point by given vector
         */
        static glm::mat4 translate(glm::vec3 translationVector);

        /**
         * Creates matrix needed to shift point by given vector.
         * @param x x component of translation vector
         * @param y y component of translation vector
         * @param z z component of translation vector
         * @return glm::mat4 needed for translating point by given vector
         */
        static glm::mat4 translate(float x, float y, float z);

        /**
         * Creates matrix ideal for scaling objects by given values
         * @param scalingVector Vector with scaling factors.
         * @return glm::mat4 matrix needed for scaling object by given values for each direction
         */
        static glm::mat4 scale(glm::vec3 scalingVector);

        /**
         * Creates matrix ideal for scaling objects by given values
         * @param x x factor
         * @param y y factor
         * @param z z factor
         * @return glm::mat4 matrix needed for scaling object by given values for each direction
         */
        static glm::mat4 scale(float x, float y, float z);

        /**
         * Creates matrix ideal for scaling objects in x,y,z directions by given value
         * @param a scaling factor
         * @return glm::mat4 matrix needed for scaling object by given value for each direction
         */
        static glm::mat4 scale(float a);
    };
}

#endif //ARCHIMEDES_TRANSFORM_H
