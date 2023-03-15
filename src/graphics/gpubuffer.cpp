#include "graphics/gpubuffer.h"

namespace arch {

void VertexArrayBuffer::fill(const Vertex *data, std::size_t size) {
    GPUBuffer<Vertex, ArrayBuffer, StaticDraw>::fill(data, size);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

}
