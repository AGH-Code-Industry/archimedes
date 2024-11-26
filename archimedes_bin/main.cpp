#include "scene/Components.h"
#include "scene/SceneManager.h"
#include <Engine.h>
#include <Logger.h>

struct VelocityComponent {
	arch::math::float3 velocity;
};

struct MyApp: arch::Application {
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
			if (transform.getPosition().y < -.5f || transform.getPosition().y > .5f) {
				velocity.velocity *= -1;
			}
			if (transform.getPosition().x < -.5f || transform.getPosition().x > .5f) {
				velocity.velocity.x *= -1;
			}

			transform.getPosition().x += velocity.velocity.x;
			transform.getPosition().y += velocity.velocity.y;
		}
	}
};

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Ref<MyApp> myApp = arch::createRef<MyApp>();

	arch::EngineConfig config{ .windowWidth = 600,
							   .windowHeight = 480,
							   .windowTitle = "Archimedes Test",
							   .backgroundColor = arch::Color(.03f, .03f, .03, 1.f),
							   .renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK };

	arch::Engine engine{ config, myApp };
	engine.start();
}
