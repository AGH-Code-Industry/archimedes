#pragma once 

#include <vector>

namespace arch::assetManager::assets {

class Shader {
public:
	Shader(std::vector<uint32_t> shaderData);

private:
	std::vector<uint32_t> _shaderData;
};

}
