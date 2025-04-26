#pragma once 

#include <Ref.h>
#include <gfx/buffer/VertexBuffer.h>
#include <gfx/buffer/IndexBuffer.h>

namespace arch::assetManager {

class Mesh {
public:
	Ref<arch::gfx::buffer::VertexBuffer> _vertexBuffer;
	Ref<arch::gfx::buffer::IndexBuffer> _indexBuffer;
	uint32_t _vertexCount;
	uint32_t _indexCount;
};

}
