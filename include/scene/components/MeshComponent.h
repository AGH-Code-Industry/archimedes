#pragma once

#include "asset/mesh/Mesh.h"

namespace arch::scene::components {

struct MeshComponent {
	// TODO: Replace with AssetRef/AssetHandle
	Ref<asset::mesh::Mesh> mesh;

	// TODO: Replace with MaterialRef/MaterialHandle
	Ref<gfx::pipeline::Pipeline> pipeline;

	bool destroyed = false;

	MeshComponent() = default;

	MeshComponent(Ref<asset::mesh::Mesh> m, Ref<gfx::pipeline::Pipeline> p) {
		mesh = std::move(m);
		pipeline = std::move(p);

		if (pipeline == nullptr || mesh == nullptr) {
			Logger::critical("WTF");
		}

		destroyed = false;
	}

	~MeshComponent() {
		/*mesh = nullptr;
		pipeline = nullptr;*/
		destroyed = true;
	}

	MeshComponent(const MeshComponent&) = default;
	MeshComponent& operator=(const MeshComponent&) = default;

	MeshComponent(MeshComponent&& other) {
		/*Logger::debug("mesh move from &p = {}, &m = {}", (void*)other.pipeline.get(), (void*)other.mesh.get());*/
		if (other.pipeline.get() == nullptr || other.mesh.get() == nullptr) {
			Logger::critical("pipeline or mesh was nullptr");
		}
		if (other.destroyed) {
			Logger::critical("other was destroyed");
		}
		/*if (other.pipeline.get() == nullptr || other.mesh.get() == nullptr) {
			int i = 0;
		} else {*/
		pipeline = std::move(other.pipeline);
		mesh = std::move(other.mesh);
		destroyed = other.destroyed;
		//}
	}

	MeshComponent& operator=(MeshComponent&& other) {
		// Logger::debug("mesh move from &p = {}, &m = {}", (void*)other.pipeline.get(), (void*)other.mesh.get());
		if (other.pipeline.get() == nullptr || other.mesh.get() == nullptr) {
			Logger::critical("pipeline or mesh was nullptr");
		}
		if (other.destroyed) {
			Logger::critical("other was destroyed");
		}
		// else {
		pipeline = std::move(other.pipeline);
		mesh = std::move(other.mesh);
		destroyed = other.destroyed;
		//}
		return *this;
	}

	void swap(MeshComponent& other) {
		std::swap(pipeline, other.pipeline);
		std::swap(mesh, other.mesh);
	}
};

} // namespace arch::scene::components
