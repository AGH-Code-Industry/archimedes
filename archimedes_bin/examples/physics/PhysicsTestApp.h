#pragma once

#include <Ecs.h>
#include <Engine.h>
#include <Scene.h>
#include <physics/System.h>
#include <physics/components/Colliding.h>

namespace physicsExample {
using namespace arch;
namespace phy = physics;
using namespace std::chrono_literals;

struct PhysicsTestApp final: Application {
	void init() override {
		const Ref<Scene> testScene = createRef<Scene>();

		// 2D square
		struct Vertex {
			float3 position;
			float2 tex_coords;
		};

		std::vector<Vertex> vertices{
			{  { 0.f, 0.f, 0.1f }, {} },
			{ { -1.f, 0.f, 0.1f }, {} },
			{  { 0.f, 1.f, 0.1f }, {} },
			{ { -1.f, 1.f, 0.1f }, {} },
		};
		std::vector<u32> indices{ 0, 1, 2, 2, 1, 3 };

		const Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();

		struct UniformBuffer {
			Mat4x4 projection;
		};

		UniformBuffer ubo{ glm::ortho(0.f, 1200.f, 0.f, 600.f) };
		auto uniformBuffer =
			renderer->getBufferManager()->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));

		const auto pipeline1 = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default2.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		});
		const auto pipeline2 = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default3.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		});

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

		Entity ground = testScene->newEntity();
		ground.addComponent(phy::BBox{
			.topLeft = float2(0),
			.bottomRight = float2{ 1200.f, -1000.f }
		 });

		auto collisionHandler = [&, &domain = testScene->domain()](const ecs::Entity lhs, const ecs::Entity rhs) {
			constexpr auto e = 0.75f;

			auto colliderOpt = domain.tryGetComponent<phy::Colliding>(lhs);
			auto rboxOpt = domain.tryGetComponent<const phy::BBox>(rhs);
			if (!colliderOpt || !rboxOpt) {
				return;
			}
			auto&& collider = *colliderOpt;
			auto&& rbox = *rboxOpt;

			auto insideDistance = (rbox.topLeft.y - collider.box.bottomRight.y);
			collider.body.velocity.y *= -e;
			collider.box.topLeft.y += insideDistance;
			collider.box.bottomRight.y += insideDistance;
			collider.body.center.position.y += insideDistance;

			Logger::debug("{}", collider.body.velocity.y);
		};
		{
			Entity box = testScene->newEntity();
			auto&& t = box.addComponent(scene::components::TransformComponent{
				.position = { 100, 500, 0 },
				.rotation = { 0, 0, 0, 1 },
				.scale = { 100, 100, 0 }
			 });
			box.addComponent(scene::components::MeshComponent{ mesh, pipeline1 });
			box.addComponent(phy::Colliding{
				.box = { .topLeft = t.position, .bottomRight = float2(t.position) + float2{ t.scale.x, -t.scale.y } },
				.body = { .center = { .mass = 1.f,
									  .position = float2(t.position) + float2{ t.scale.x, -t.scale.y } / 2.f },
						  .force = { 0, -98.1f },
						  .velocity = { 0, 0 } },
				.action = collisionHandler
			   });
		}

		/*const ecs::Entity e1 = testScene->newEntity();
		float2 position{ 100, 200 };
		testScene->domain().addComponent<scene::components::TransformComponent>(
			e1,
			{
				{ position, 0 },
				{ 0.f, 0.f, 0.f, 1.f },
				float3(100, 100, 0)
		}
		);
		testScene->domain().addComponent<scene::components::MeshComponent>(e1, { mesh, pipeline1 });
		testScene->domain().addComponent(
			e1,
			phy::Colliding{
				.box = phy::BBox{.topLeft = position,.bottomRight = position + float2{ 100, -100 } },
				.body =
					phy::Moveable{
								 { 1.f, position + float2{ 100, -100 } / 2.f },
								 { 0.f, 0.f }, { 50.f, 0.f },
								 },
				.action = ideallyElasticCollision
		  }
		);

		const ecs::Entity e2 = testScene->newEntity();
		position = { 1'000, 200 };
		testScene->domain().addComponent<scene::components::TransformComponent>(
			e2,
			{
				{ position, 0 },
				{ 0.f, 0.f, 0.f, 1.f },
				float3(100, 100, 0)
		}
		);
		testScene->domain().addComponent<scene::components::MeshComponent>(e2, { mesh, pipeline2 });
		testScene->domain().addComponent(
			e2,
			phy::Colliding{
				.box = phy::BBox{ .topLeft = position, .bottomRight = position + float2{ 100, -100 } },
				.body = phy::Moveable{ { 1.f, position + float2{ 100, -100 } / 2.f }, { 0.f, 0.f }, { -50.f, 0.f } },
				.action = ideallyElasticCollision
		  }
		);*/

		scene::SceneManager::get()->changeScene(testScene);
		_physicsSystem = createRef<phy::System>(std::ref(testScene->domain()));
	}

	void update() override {
		std::this_thread::sleep_for(16ms);

		auto view = scene::SceneManager::get()
						->currentScene()
						->domain()
						.view<scene::components::TransformComponent, phy::Colliding>();

		for (auto [entity, transform, colliding] : view.all()) {
			transform.position = {
				colliding.body.center.position - float2{ transform.scale.x, -transform.scale.y }
					 / 2.f,
				0
			};
		}

		_physicsSystem->update();
	}

private:
	Ref<phy::System> _physicsSystem;
};
} // namespace physicsExample
