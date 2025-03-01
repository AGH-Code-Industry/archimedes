#pragma once

#include <Ecs.h>
#include <Engine.h>
#include <Scene.h>
#include <stb_image.h>

using namespace arch;

class TextRenderTestApp: public Application {
	void init() override {
		Ref<Scene> testScene = createRef<Scene>();

		// 2D square
		struct Vertex {
			float3 position;
			float2 tex_coords;
		};

		stbi_set_flip_vertically_on_load(true);
		int atlasWidth, atlasHeight, nChannels;
		unsigned char* data = stbi_load("atlas.png", &atlasWidth, &atlasHeight, &nChannels, STBI_rgb_alpha);
		assert(data);

		auto atlasPixels = std::vector<Color>(atlasWidth * atlasHeight);
		for (u32 i = 0; i != atlasPixels.size(); ++i) {
			atlasPixels[i].r = (float)data[4 * i + 0] / 255.f;
			atlasPixels[i].g = (float)data[4 * i + 1] / 255.f;
			atlasPixels[i].b = (float)data[4 * i + 2] / 255.f;
			atlasPixels[i].a = (float)data[4 * i + 3] / 255.f;
		}
		stbi_image_free(data);

		std::vector<Vertex> vertices{
			{  { 0.f, 0.f, 0.f }, { 1.f, 1.f } },
			{  { 0.f, 1.f, 0.f }, { 1.f, 0.f } },
			{ { 1.f, 0.0f, 0.f }, { 0.f, 1.f } },
			{ { 1.f, 1.0f, 0.f }, { 0.f, 0.f } },
		};
		std::vector<u32> indices{ 0, 1, 2, 1, 3, 2 };

		Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();

		auto atlasTexture = renderer->getTextureManager()->createTexture2D(atlasWidth, atlasHeight, atlasPixels.data());

		Ref<asset::mesh::Mesh> mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);
		auto pipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_atlas.glsl",
				.textures = { atlasTexture },
				.buffers = {},
			}
		);

		{
			auto e = testScene->newEntity();
			auto&& t = e.addComponent<scene::components::TransformComponent>({
				{ 600.0f, 300.0f, 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f },
				{ 200.0f, 200.f, 0.0f },
			});
			// t.scale *= 1'000;
			e.addComponent<scene::components::MeshComponent>({ mesh, pipeline });
			arch::Logger::debug("{:|v}\n", e.handle());
		}

		scene::SceneManager::get()->changeScene(testScene);
	}
};
