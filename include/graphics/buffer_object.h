#include "graphics/primitives.h"

#include <glad/glad.h>

#include <optional>
#include <spdlog/spdlog.h>

namespace arch {

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
        other._VBO.reset();
    }

    BufferObject& operator=(BufferObject&) = delete;

    BufferObject& operator=(BufferObject &&other) noexcept {
        if (this == &other)
            return *this;
        clear();
        _VBO = other._VBO;
        other._VBO.reset();
    }

    ~BufferObject() {
        clear();
    }

    void fill(const Data *data, std::size_t size) {
        clear();
        _VBO = 0;
        glGenBuffers(1, &_VBO.value());
        glBindBuffer(target, _VBO.value());
        glBufferData(target, sizeof(Data) * size, data, usage);
    }
protected:
    std::optional<unsigned int> _VBO;
private:
    void clear() {
        if (!_VBO.has_value())
            return;
        glDeleteBuffers(1, &_VBO.value());
        _VBO.reset();
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
