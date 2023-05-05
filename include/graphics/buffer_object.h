#include "graphics/primitives.h"

#include <glad/glad.h>

#include <optional>
#include <cassert>

namespace arch {

using ObjectName = GLuint;

enum BufferTarget {
    Array = GL_ARRAY_BUFFER,
    ElementArray = GL_ELEMENT_ARRAY_BUFFER
};

enum BufferUsage {
    StaticDraw = GL_STATIC_DRAW
};

template <typename Data, BufferTarget target, BufferUsage usage>
class BufferObject {
public:
    BufferObject() = default;
    BufferObject(const BufferObject&) = delete;

    BufferObject(BufferObject &&other) noexcept {
        _VBO = other._VBO;
        other._VBO = INVALID;
    }

    BufferObject& operator=(BufferObject&) = delete;

    BufferObject& operator=(BufferObject &&other) noexcept {
        if (this == &other)
            return *this;
        clear();
        _VBO = other._VBO;
        other._VBO = INVALID;
    }

    ~BufferObject() {
        clear();
    }

    void fill(const Data *data, std::size_t size) {
        clear();
        glGenBuffers(1, &_VBO);
        bind();
        glBufferData(target, sizeof(Data) * size, data, usage);
        unbind();
    }

    void bind() {
        assert(_VBO != INVALID && "Invalid buffer object has been bound to.");
        glBindBuffer(target, _VBO);
    }

    void unbind() {
        glBindBuffer(target, INVALID);
    }

    static constexpr ObjectName INVALID = 0;
protected:
    ObjectName _VBO {INVALID};

private:
    void clear() {
        if (_VBO == 0)
            return;
        glDeleteBuffers(1, &_VBO);
        _VBO = INVALID;
    }

};

using VertexBuffer = BufferObject<Vertex, BufferTarget::Array, BufferUsage::StaticDraw>;
using ElementBuffer = BufferObject<Index, BufferTarget::ElementArray, BufferUsage::StaticDraw>;

class VertexArray {
public:
    VertexArray();
    ~VertexArray();
    VertexArray(VertexArray&) = delete;
    VertexArray(VertexArray&& other);
    VertexArray& operator=(VertexArray&) = delete;
    VertexArray& operator=(VertexArray&& other);

    void enable();
    void bind() const;
private:
    unsigned int _VAO {0};
};


}
