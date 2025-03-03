#pragma once

#include <Ecs.h>
#include <Engine.h>
#include <Scene.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace arch;

class TextRenderTestApp: public Application {
	void init() override {
		int windowWidth = 800;
		int windowWidth = 1'200;
		int windowHeight = 600;

		Ref<Scene> testScene = createRef<Scene>();

		// 2D square
		struct Vertex {
			float3 position;
			float2 tex_coords;
		};

		std::vector<Vertex> vertices{
			{  { 0.f, 0.f, 0.1f }, { 0.f, 1.f } },
			{ { -1.f, 0.f, 0.1f }, { 1.f, 1.f } },
			{  { 0.f, 1.f, 0.1f }, { 0.f, 0.f } },
			{ { -1.f, 1.f, 0.1f }, { 1.f, 0.f } },
		};
		std::vector<u32> indices{ 0, 1, 2 };
		std::vector<u32> atlasIndices{ 0, 1, 2, 2, 1, 3 };

		// Ref<Shader> vShader = Shader::load("shaders/vertex_shader.sprv");
		// Ref<Shader> fShader = Shader::load("shaders/fragment_shader.sprv");
		// Ref<Material> material = Material::create(vShader, fShader);
		// material->setTexture("_mainTxt", TextureLoader::read_file("textures/.jpg"));
		// material->SetFloat("_mixValue", 0.2f);
		// material->SetFloat3("_pos", glm::vec3(0.5f, 0.5f, 0.5f));
		// material->SetColor("_color", glm::vec3(1.0f, 0.0f, 0.0f));

		Color pixels[] = {
			Color{ 1, .5, 1, 1 }
		};

		Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();

		Ref<gfx::texture::Texture> texture = renderer->getTextureManager()->createTexture2D(1, 1, pixels);

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

		auto atlasTexture = renderer->getTextureManager()->createTexture2D(atlasWidth, atlasHeight, atlasPixels.data());

		Ref<asset::mesh::Mesh> atlasMesh = asset::mesh::Mesh::create<Vertex>(vertices, atlasIndices);

		struct UniformBuffer {
			Mat4x4 projection;
		};

		UniformBuffer ubo{ glm::ortho(0.f, (float)windowWidth, 0.f, (float)windowHeight) };
		auto uniformBuffer =
			renderer->getBufferManager()->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));

		auto pipeline = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = { texture },
			.buffers = { uniformBuffer },
		});
		auto pipeline2 = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default2.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		});
		auto atlasPipeline = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_atlas.glsl",
			.textures = { atlasTexture },
			.buffers = { uniformBuffer },
		});

		Ref<asset::mesh::Mesh> mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);

		{
			auto e = testScene->newEntity();
			auto&& t = e.addComponent<scene::components::TransformComponent>({
				{ 0, windowHeight, 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f },
				{ windowHeight, windowHeight, 0.0f },
			});
			// t.scale *= 1'000;
			e.addComponent<scene::components::MeshComponent>({ atlasMesh, atlasPipeline });
			arch::Logger::debug("{:|v}\n", e.handle());
		}

		scene::SceneManager::get()->changeScene(testScene);
	}
};
