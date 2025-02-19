#pragma once

#include <Ecs.h>
#include <Engine.h>
#include <Scene.h>

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

		Ref<asset::mesh::Mesh> mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);

		{
			ecs::Entity e = testScene->newEntity();
			testScene->domain().addComponent<scene::components::TransformComponent>(
				e,
				{
					{ 0.0f, 0.0f, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 1.0f },
					{ 1.0f, .5f, 0.0f },
			}
			);

			testScene->domain().addComponent<scene::components::MeshComponent>(e, { mesh });
			testScene->domain().addComponent<VelocityComponent>(e, float3{ 0.01f, .01f, 0.0f });
		}

		for (int i = 0; i < 5; i++) {
			ecs::Entity e = testScene->newEntity();
			testScene->domain().addComponent<scene::components::TransformComponent>(
				e,
				{
					{ -1 + 0.5f * i, 0.25f * i, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 1.0f },
					float3(1)
			  }
			);
			testScene->domain().addComponent<scene::components::MeshComponent>(e, { mesh });
			testScene->domain().addComponent<VelocityComponent>(e, float3{ 0.0f, -.01f, 0.0f });
		}

		scene::SceneManager::get()->changeScene(testScene);
	}

	void update() override {
		auto view = scene::SceneManager::get()
						->currentScene()
						->domain()
						.view<scene::components::TransformComponent, VelocityComponent>();

		for (auto [entity, transform, velocity] : view.all()) {
			if ((transform.position.y < -.5f && velocity.velocity.y < 0) ||
				(transform.position.y > .5f && velocity.velocity.y > 0)) {
				velocity.velocity.y *= -1;
			}
			if ((transform.position.x < -.5f && velocity.velocity.x < 0) ||
				(transform.position.x > .5f && velocity.velocity.x > 0)) {
				velocity.velocity.x *= -1;
			}

			transform.position.x += velocity.velocity.x;
			transform.position.y += velocity.velocity.y;
		}
	}
};
