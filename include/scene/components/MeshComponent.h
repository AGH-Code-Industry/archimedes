#pragma once

// #include "asset/mesh/Mesh.h"
#include "asset_manager/AssetHandle.h"
#include "asset_manager/assets/Mesh.h"

namespace arch::scene::components {

struct MeshComponent {
	Ref<arch::assetManager::AssetHandle<arch::assetManager::assets::Mesh>> assetHandle;

	// TODO: Replace with MaterialRef/MaterialHandle
	Ref<gfx::pipeline::Pipeline> pipeline;
};

} // namespace arch::scene::components
