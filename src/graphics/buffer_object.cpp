#include "graphics/buffer_object.h"

namespace arch {

VertexArray::VertexArray() {
    glGenVertexArrays(1, &_VAO);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &_VAO);
}

VertexArray::VertexArray(VertexArray&& other) {
    _VAO = other._VAO;
    other._VAO = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) {
    if (this == &other)
        return *this;
    _VAO = other._VAO;
    other._VAO = 0;
    return *this;
}

void VertexArray::enable() {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    glBindVertexArray(0);
}

void VertexArray::bind() const {
    glBindVertexArray(_VAO);
}

}
