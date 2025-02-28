#pragma once
#include <string>
#include <vector>

#include "Ref.h"

namespace arch::gfx::buffer {
class Buffer;
}

namespace arch::gfx::texture {
class Texture;
}

namespace arch::gfx::pipeline {

class Pipeline {
public:
	struct Desc {
		std::string vertexShaderPath;
		std::string fragmentShaderPath;

		std::vector<Ref<texture::Texture>> textures;
		std::vector<Ref<buffer::Buffer>> buffers;
	};

public:
	explicit Pipeline(const Desc& desc): _desc(desc) {}

	virtual ~Pipeline() = default;

protected:
	Desc _desc;
};

} // namespace arch::gfx::pipeline
