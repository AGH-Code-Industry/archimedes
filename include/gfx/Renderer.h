#pragma once

/// @brief Temporary code

#include <volk.h>

namespace arch::gfx {

class Renderer {
public:
	void init();

private:
	VkInstance _instance;
};

}
