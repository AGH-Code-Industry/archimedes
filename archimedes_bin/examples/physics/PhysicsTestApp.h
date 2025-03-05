#pragma once

#include <Ecs.h>
#include <Engine.h>
#include <Scene.h>
#include <physics/System.h>
#include <physics/components/Colliding.h>

namespace physicsExample {
using namespace arch;
namespace phy = physics;

struct PhysicsTestApp final: Application {
	void init() override {
		const Ref<Scene> testScene = createRef<Scene>();

		// 2D square
		struct Vertex {
			float3 position;
			float2 tex_coords;
		};

		std::vector<Vertex> vertices{
			{ { -.25f, -.25f, 0.1f }, { 0.f, 0.f } },
			{ { 0.f, -.25f, 0.1f }, { 1.f, 0.f } },
			{ { 0.f, 0.f, 0.1f }, { 1.f, 1.f } },
			{ { -.25f, 0.f, 0.1f }, { 0.f, 1.f } },
		};
		std::vector<u32> indices{ 0, 3, 2, 2, 1, 0 };

		const Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();

		struct UniformBuffer {
			Mat4x4 projection;
		};

		UniformBuffer ubo{ glm::ortho(0.f, 640.f, 0.f, 400.f) };
		auto uniformBuffer =
			renderer->getBufferManager()->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));

		const auto pipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default2.glsl",
				.textures = {},
				.buffers = { uniformBuffer },
			}
		);

		const Ref<asset::mesh::Mesh> mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);

		auto ideallyElasticCollision = [&,
										&domain = testScene->domain()](const ecs::Entity me, const ecs::Entity other) {
			phy::Moveable& myBody = domain.getComponent<phy::Colliding>(me).body;
			phy::Moveable& otherBody = domain.getComponent<phy::Colliding>(other).body;

			float2 v1 = myBody.velocity * (myBody.center.mass - otherBody.center.mass);
			v1 += 2 * otherBody.center.mass * otherBody.velocity;
			v1 /= myBody.center.mass + otherBody.center.mass;

			float2 v2 = otherBody.velocity * (otherBody.center.mass - myBody.center.mass);
			v2 += 2 * myBody.center.mass * myBody.velocity;
			v2 /= myBody.center.mass + otherBody.center.mass;

			myBody.velocity = v1;
			otherBody.velocity = v2;
		};

		const ecs::Entity e1 = testScene->newEntity();
		float2 position{ -.875f, 0.f };
		testScene->domain().addComponent<scene::components::TransformComponent>(
			e1,
			{
				{ position, 0 },
				{ 0.f, 0.f, 0.f, 1.f },
				float3(1)
		  }
		);
		testScene->domain().addComponent<scene::components::MeshComponent>(e1, { mesh, pipeline });
		testScene->domain().addComponent(
			e1,
			phy::Colliding{
				.box = phy::BBox{.topLeft = position,.bottomRight = position + float2{ .25f, -.25f } },
				.body =
					phy::Moveable{
								 { 1.f, position },
								 { 0.f, 0.f },	{ 0.1f, 0.f },
								 },
				.action = ideallyElasticCollision
		  }
		);

		const ecs::Entity e2 = testScene->newEntity();
		position = { .75f, 0.f };
		testScene->domain().addComponent<scene::components::TransformComponent>(
			e2,
			{
				{ position, 0 },
				{ 0.f, 0.f, 0.f, 1.f },
				float3(1)
		  }
		);
		testScene->domain().addComponent<scene::components::MeshComponent>(e2, { mesh, pipeline });
		testScene->domain().addComponent(
			e2,
			phy::Colliding{
				.box = phy::BBox{ .topLeft = position, .bottomRight = position + float2{ .25f, -.25f } },
				.body = phy::Moveable{ { 5.f, position }, { 0.f, 0.f }, { -0.1f, 0.f } },
				.action = ideallyElasticCollision
		  }
		);

		scene::SceneManager::get()->changeScene(testScene);
		_physicsSystem = createRef<phy::System>(std::ref(testScene->domain()));
	}

	void update() override {
		auto view = scene::SceneManager::get()
						->currentScene()
						->domain()
						.view<scene::components::TransformComponent, phy::Colliding>();

		for (auto [entity, transform, colliding] : view.all()) {
			transform.position = { colliding.body.center.position, 0 };
		}

		_physicsSystem->update();
	}

private:
	Ref<phy::System> _physicsSystem;
};
} // namespace physics_example
