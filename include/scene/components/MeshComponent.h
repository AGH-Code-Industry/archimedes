#pragma once

#include "asset/mesh/Mesh.h"

namespace arch::scene::components {

struct MeshComponent {
	// TODO: Replace with AssetRef/AssetHandle
	Ref<asset::mesh::Mesh> mesh;
};

} // namespace arch::scene::components
