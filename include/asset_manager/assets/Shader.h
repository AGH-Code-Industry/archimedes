#pragma once 

#include <vector>

namespace arch::assetManager::assets {

struct Shader {
	Shader(std::vector<uint32_t> shaderData): _shaderData(std::move(shaderData)) {}
	std::vector<uint32_t> _shaderData;
};

}
