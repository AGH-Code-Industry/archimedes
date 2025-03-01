#pragma once

#include <Ecs.h>
#include <Engine.h>
#include <Scene.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace arch;

struct VelocityComponent {
	float3 velocity;
};

class NvrhiRendererTestApp: public Application {
	void init() override {
		Ref<Scene> testScene = createRef<Scene>();

		// 2D square
		struct Vertex {
			float3 position;
			float2 tex_coords;
		};

		std::vector<Vertex> vertices{
			{ { -.25f, -.25f, 0.1f }, { 0.f, 0.f } },
			{	  { 0.f, .25f, 0.1f }, { .5f, 1.f } },
			{  { .25f, -.25f, 0.1f }, { 1.f, 0.f } },
		};
		std::vector<u32> indices{ 0, 1, 2 };

		// Ref<Shader> vShader = Shader::load("shaders/vertex_shader.sprv");
		// Ref<Shader> fShader = Shader::load("shaders/fragment_shader.sprv");
		// Ref<Material> material = Material::create(vShader, fShader);
		// material->setTexture("_mainTxt", TextureLoader::read_file("textures/.jpg"));
		// material->SetFloat("_mixValue", 0.2f);
		// material->SetFloat3("_pos", glm::vec3(0.5f, 0.5f, 0.5f));
		// material->SetColor("_color", glm::vec3(1.0f, 0.0f, 0.0f));

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

		// std::vector<Vertex> atlasVertices{
		//	{ { 0.f, 0.f, 0.f }, { 0.f, 0.f } },
		//	{ { 1.f, 0.f, 0.f }, { 1.f, 0.f } },
		//	{ { 0.f, 1.f, 0.f }, { 0.f, 1.f } },
		//	//{ { 1.f, 1.f, 0.f }, { 1.f, 1.f } },
		// };
		std::vector<Vertex> vertices2{
			{  { 0.f, 0.f, 0.f }, { 1.f, 1.f } },
			{  { 0.f, 1.f, 0.f }, { 1.f, 0.f } },
			{ { 1.f, 0.0f, 0.f }, { 0.f, 1.f } },
			{ { 1.f, 1.0f, 0.f }, { 0.f, 0.f } },
		};
		std::vector<u32> atlasIndices{ 0, 1, 2, 1, 3, 2 };

		Color pixels[] = {
			Color{ 1, .5, 1, 1 }
		};

		Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();

		Ref<gfx::texture::Texture> texture = renderer->getTextureManager()->createTexture2D(1, 1, pixels);
		auto atlasTexture = renderer->getTextureManager()->createTexture2D(atlasWidth, atlasHeight, atlasPixels.data());

		auto pipeline = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default2.glsl",
			.textures = { texture },
			.buffers = {},
		});
		auto pipeline2 = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default2.glsl",
			.textures = {},
			.buffers = {},
		});

		Ref<asset::mesh::Mesh> atlasMesh = asset::mesh::Mesh::create<Vertex>(vertices2, atlasIndices);
		auto atlasPipeline = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_atlas.glsl",
			.textures = { atlasTexture },
			.buffers = {},
		});

		Ref<asset::mesh::Mesh> mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);

		{
			auto e = testScene->newEntity();
			auto&& t = e.addComponent<scene::components::TransformComponent>({
				{ 600.0f, 300.0f, 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f },
				{ 200.0f, 200.f, 0.0f },
			});
			// t.scale *= 1'000;
			e.addComponent<scene::components::MeshComponent>({ atlasMesh, atlasPipeline });
			arch::Logger::debug("{:|v}\n", e.handle());
		}

		{
			ecs::Entity e = testScene->newEntity();
			auto&& t = testScene->domain().addComponent<scene::components::TransformComponent>(
				e,
				{
					{ 320.0f, 200.0f, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 1.0f },
					{ 1.0f, .5f, 0.0f },
			}
			);
			t.scale *= 50;

			testScene->domain().addComponent<scene::components::MeshComponent>(e, { mesh, pipeline });
			// testScene->domain().addComponent<VelocityComponent>(e, float3{ 0.01f, .01f, 0.0f });
		}

		for (int i = 0; i < 5; i++) {
			ecs::Entity e = testScene->newEntity();
			auto&& t = testScene->domain().addComponent<scene::components::TransformComponent>(
				e,
				{
					{ 320.f, 200.f, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 1.0f },
					{ 1, 1, 0 }
			}
			);
			t.scale *= 50;
			testScene->domain().addComponent<scene::components::MeshComponent>(e, { mesh, pipeline2 });
			// testScene->domain().addComponent<VelocityComponent>(e, float3{ .0f, .0f, 0.0f });
		}

		scene::SceneManager::get()->changeScene(testScene);
	}

	void update() override {
		static float frame = 0;
		frame += 0.005;
		auto view = scene::SceneManager::get()->currentScene()->domain().view<scene::components::TransformComponent>();

		for (auto&& [entity, transform] : view.all()) {
			/*if ((transform.position.y < 0 && velocity.velocity.y < 0) ||
				(transform.position.y > 1 * 50 && velocity.velocity.y > 0)) {
				velocity.velocity.y *= -1;
			}
			if ((transform.position.x < 0 && velocity.velocity.x < 0) ||
				(transform.position.x > 1 * 50 && velocity.velocity.x > 0)) {
				velocity.velocity.x *= -1;
			}*/

			/*transform.scale.x = */ // transform.scale.y = std::cos(frame) * 1'000;
		}
	}
};
