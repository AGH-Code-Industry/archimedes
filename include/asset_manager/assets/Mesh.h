#pragma once

#include <span>
#include <vector>

#include "Ref.h"

namespace arch::assetManager::assets {

struct Mesh {
	Mesh(std::vector<float> vertices, std::vector<uint32_t> indices):
		_vertexBuffer(std::move(vertices)),
		_indexBuffer(std::move(indices)) {}

	std::vector<float> _vertexBuffer;
	std::vector<uint32_t> _indexBuffer;
};

} // namespace arch::assetManager::assets
