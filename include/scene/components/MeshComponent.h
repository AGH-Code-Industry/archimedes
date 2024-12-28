#pragma once

#include "gfx/Mesh.h"

namespace arch::scene::components {

struct MeshComponent {
	Ref<gfx::Mesh> mesh;
};

} // namespace arch::scene::components
