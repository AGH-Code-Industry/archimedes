#pragma once

#include <codecvt>
#include <glm/gtx/string_cast.hpp>
#include <locale>
#include <numbers>
#include <random>

#include <archimedes/Camera.h>
#include <archimedes/Ecs.h>
#include <archimedes/Engine.h>
#include <archimedes/Input.h>
#include <archimedes/Scene.h>
#include <archimedes/Text.h>
#include <archimedes/BuildInfo.h>

using namespace arch;


std::vector<f32> generatePerlinHeights(u32 width, f32 maxHeight) {
	std::vector<f32> heights;
	heights.reserve(width);
	for (u32 i = 0; i < width; i++){
		f32 height = (PerlinNoise1D::generate(i) + 1.0f) / 2.0f * maxHeight;
		heights.push_back(height);
	}
	return heights;
}


class PerlinNoise1DTestApp: public Application {


	const f32 mapWidth = 1280;
	const f32 mapHeight = 600;

	const f32 mapChunks = 400;


	std::vector<scene::Entity> mapEntities;
	Ref<asset::mesh::Mesh> mesh;
	Ref<Scene> scene;
	Ref<gfx::Renderer> renderer;
	Ref<gfx::texture::Texture> texture;

	void generateMap(f32 chunks){

		if(!mapEntities.empty()){
			for(auto entity: mapEntities){
				scene->removeEntity(entity);
			}
			mapEntities.clear();
		}
		

		PerlinNoise1D::build(128, -0.5f, 0.5f);

		auto heights = generatePerlinHeights(chunks, mapHeight);

		auto buffer = scene->domain().global<Camera>().buffer();
		auto pipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = { texture },
				// Use the view-projection matrix from camera
				// Ordering may differ depending on the used shader
				.buffers = { buffer },
			}
		);


		f32 chunkWidth = mapWidth / chunks;
		for(i32 i = 0; i < chunks; i++){
			f32 x = -mapWidth / 2 + chunkWidth * (f32)i;
			auto entity = scene->newEntity();
			entity.addComponent(
				scene::components::TransformComponent{
					.position = { x, -mapHeight / 2.0, 0 },
					.rotation = quaternion(0.0f),
					.scale = { chunkWidth, heights[i], 1 }
				}
			);
			entity.addComponent(scene::components::MeshComponent{ .mesh = mesh, .pipeline = pipeline });
			mapEntities.push_back(entity);

		}
	}

	void init() override {
		scene = createRef<Scene>();

		scene::SceneManager::get()->changeScene(scene);

		// 2D square
		struct Vertex {
			float3 position;
			float2 tex_coords;
		};

		std::vector<u32> indices{
			0, 1, 2, 2, 1, 3,
		};

		std::vector<Vertex> vertices{
			{  { 0.f, 0.f, 0.f }, {1, 0} },
			{ { -1.f, 0.f, 0.f }, {0, 0} },
			{  { 0.f, 1.f, 0.f }, {1, 1} },
			{ { -1.f, 1.f, 0.f }, {0, 1} },
		};

		Color pixels[1] = {
			Color{ 1, .5, 1, 1 }
		};

		renderer = gfx::Renderer::getCurrent();
		texture = renderer->getTextureManager()->createTexture2D(1, 1, pixels);
		mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);

		// Create camera
		scene->domain().global<Camera>();

		// Initialize Perlin Noise params
		PerlinNoise1D::minResult = -1.0f;
		PerlinNoise1D::maxResult = 1.0f;
		PerlinNoise1D::baseAmplitude = 1.0f;
		PerlinNoise1D::baseFrequency = 0.1f;

		generateMap(mapChunks);

	}

	void update() {
		if(input::Keyboard::space.down()){
			generateMap(mapChunks);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

};
