#pragma once

#include <span>
#include <vector>

#include "Ref.h"
#include "gfx/Buffer.h"
#include "gfx/Renderer.h"
#include "gfx/buffer/BufferManager.h"

namespace arch::assetManager::assets {

class Mesh {
public:
	Mesh(std::vector<float> vertices, std::vector<uint32_t> indices):
		_vertexBuffer(std::move(vertices)),
		_indexBuffer(std::move(indices)) {}

private:
	std::vector<float> _vertexBuffer;
	std::vector<uint32_t> _indexBuffer;
};

} // namespace arch::assetManager::assets
