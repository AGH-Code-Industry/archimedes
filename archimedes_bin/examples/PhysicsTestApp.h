#pragma once

#include <Ecs.h>
#include <Engine.h>
#include <Scene.h>
#include <physics/components/Movable.h>
#include <physics/System.h>

using namespace arch;
namespace phy = physics;

struct PhysicsTestApp: Application {
	void init() override {
		Ref<Scene> testScene = createRef<Scene>();

		// 2D square
		struct Vertex {
			float3 position;
			float2 tex_coords;
		};

		std::vector<Vertex> vertices{
			{ { -.25f, -.25f, 0.1f }, { 0.f, 0.f } },
			{	  { 0.f, -.25f, 0.1f }, { 1.f, 0.f } },
			{	  { 0.f, 0.f, 0.1f }, { 1.f, 1.f } },
			{	  { -.25f, 0.f, 0.1f }, { 0.f, 1.f } },
		};
		std::vector<u32> indices{ 0, 3, 2, 2, 1, 0 };

		Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();
	auto pipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default2.glsl",
				.textures = {},
				.buffers = {},
			}
		);

		Ref<asset::mesh::Mesh> mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);

		ecs::Entity e = testScene->newEntity();
		float2 position{ -1.f, 0.f};
		testScene->domain().addComponent<scene::components::TransformComponent>(
			e,
			{
				{position, 0},
				{ 0.f, 0.f, 0.f, 1.f },
				float3(1)
		  }
		);
		testScene->domain().addComponent<scene::components::MeshComponent>(e, { mesh, pipeline });
		testScene->domain().addComponent(e, phy::Movable{{1.f, position}, {0.f, 0.f}, {0.1f, 0.f}});

		scene::SceneManager::get()->changeScene(testScene);
		_physicsSystem = createRef<phy::System>(std::ref(testScene->domain()));
	}

	void update() override {
		auto view = scene::SceneManager::get()
						->currentScene()
						->domain()
						.view<scene::components::TransformComponent, phy::Movable>();

		for (auto [entity, transform, movable] : view.all()) {
			transform.position = {movable.center.position, 0};
		}

		_physicsSystem->update();
	}
private:
	Ref<phy::System> _physicsSystem;
};
