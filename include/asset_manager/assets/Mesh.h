#pragma once

#include <span>

#include "gfx/Buffer.h"
#include "Ref.h"
#include "gfx/Renderer.h"
#include "gfx/buffer/BufferManager.h"

namespace arch::assetManager::assets {

class Mesh {
public:
	Mesh(std::span<float> vertices, std::span<uint32_t> indices);
private:
	Ref<gfx::VertexBuffer> _vertexBuffer;
	Ref<gfx::IndexBuffer> _indexBuffer;
};

} // namespace arch::assetManager::assets
