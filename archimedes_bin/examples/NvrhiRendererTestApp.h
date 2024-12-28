#pragma once

#include <Ecs.h>
#include <Engine.h>
#include <Scene.h>

struct VelocityComponent {
	arch::math::float3 velocity;
};

class NvrhiRendererTestApp: public arch::Application {
	void init() override {
		arch::Ref<arch::scene::Scene> testScene = arch::createRef<arch::scene::Scene>();

		{
			arch::ecs::Entity e = testScene->newEntity();
			testScene->domain().addComponent<arch::scene::components::TransformComponent>(
				e,
				{
					{ 0.0f, 0.0f, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 1.0f },
					{ 1.0f, .5f, 0.0f },
			}
			);

			// 2D square
			// struct Vertex {
			// 	float3 position;
			// 	float3 color;
			// 	float2 tex_coords;
			// };

			// std::vector<Vertex> vertices{
			// 	{  float3(0.5f,  0.5f, 0.0f), {}, float2(1.0f, 1.0f) },
			// 	{  float3(0.5f, -0.5f, 0.0f), {}, float2(1.0f, 0.0f) },
			// 	{ float3(-0.5f, -0.5f, 0.0f), {}, float2(0.0f, 0.0f) },
			// 	{ float3(-0.5f,	0.5f, 0.0f), {}, float2(0.0f, 1.0f) }
			// };
			// std::vector<u32> indices{ 0, 1, 3, 1, 2, 3 };

			// Ref<Shader> vShader = Shader::load("shaders/vertex_shader.sprv");
			// Ref<Shader> fShader = Shader::load("shaders/fragment_shader.sprv");
			// Ref<Material> material = Material::create(vShader, fShader);
			// material->setTexture("_mainTxt", TextureLoader::read_file("textures/.jpg"));
			// material->SetFloat("_mixValue", 0.2f);
			// material->SetFloat3("_pos", glm::vec3(0.5f, 0.5f, 0.5f));
			// material->SetColor("_color", glm::vec3(1.0f, 0.0f, 0.0f));

			// Ref<Mesh> mesh = Mesh::create<Vertex>(vertices, indices);
			testScene->domain().addComponent<arch::scene::components::MeshComponent>(e, { /*mesh*/ });
			testScene->domain().addComponent<VelocityComponent>(e, arch::float3{ 0.0f, .01f, 0.0f });
		}

		{
			arch::ecs::Entity e = testScene->newEntity();
			testScene->domain().addComponent<arch::scene::components::TransformComponent>(
				e,
				{
					{ 0.5f, 0.5f, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 1.0f },
					arch::float3(1)
			}
			);
			testScene->domain().addComponent<arch::scene::components::MeshComponent>(e, { /*mesh*/ });
			testScene->domain().addComponent<VelocityComponent>(e, arch::float3{ 0.0f, -.01f, 0.001f });
		}

		arch::scene::SceneManager::get()->changeScene(testScene);
	}

	void update() override {
		auto view = arch::scene::SceneManager::get()
						->currentScene()
						->domain()
						.view<arch::scene::components::TransformComponent, VelocityComponent>();

		for (auto [entity, transform, velocity] : view.all()) {
			if (transform.position.y < -.5f || transform.position.y > .5f) {
				velocity.velocity *= -1;
			}
			if (transform.position.x < -.5f || transform.position.x > .5f) {
				velocity.velocity.x *= -1;
			}

			transform.position.x += velocity.velocity.x;
			transform.position.y += velocity.velocity.y;
		}
	}
};
