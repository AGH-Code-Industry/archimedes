#include "graphics/primitives.h"

#include <glad/glad.h>

#include <optional>

namespace arch {

enum BufferTarget {
    ArrayBuffer = GL_ARRAY_BUFFER
};

enum BufferUsage {
    StaticDraw = GL_STATIC_DRAW
};

template <typename Data, BufferTarget target, BufferUsage usage>
class GPUBuffer {
public:
    GPUBuffer() {
        //It is necessary to do things like that in order to reconcile differences
        //between modern C++ and C OpenGL API.
        glGenBuffers(1, &_VBO.value());
    }

    GPUBuffer(const GPUBuffer&) = delete;

    GPUBuffer(GPUBuffer &&other) noexcept {
        _VBO = other._VBO;
        other._VBO.reset();
    }

    GPUBuffer& operator=(GPUBuffer&) = delete;

    GPUBuffer& operator=(GPUBuffer &&other) noexcept {
        if (this == &other)
            return *this;
        glDeleteBuffers(1, &_VBO.value());
        _VBO = other._VBO;
        other._VBO.reset();
    }

    ~GPUBuffer() {
        glDeleteBuffers(1, &_VBO.value());
    }

    virtual void fill(const Data *data, std::size_t size) {
        glBindBuffer(target, _VBO.value());
        glBufferData(target, sizeof(Data) * size, data, usage);
    }
protected:
    std::optional<unsigned int> _VBO;
};

class VertexArrayBuffer : public GPUBuffer<Vertex, ArrayBuffer, StaticDraw> {
public:
    void fill(const Vertex *data, std::size_t size) override;
};

}
