#pragma once

#include <archimedes/physics/RigidBodyComponent.h>
#include <archimedes/Engine.h>
#include <archimedes/Scene.h>
#include <archimedes/physics/ColliderComponent.h>
#include <archimedes/physics/System.h>

namespace physicsExample {
using namespace arch;
namespace phy = physics;

struct RepulseTestApp final: Application {
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

		UniformBuffer ubo{ glm::mat4{1} };
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

		auto ideallyElasticCollision = [&, &domain = testScene->domain()](const ecs::Entity me, const ecs::Entity other) {
			phy::RigidBodyComponent& myBody = domain.getComponent<phy::RigidBodyComponent>(me);
			phy::RigidBodyComponent& otherBody = domain.getComponent<phy::RigidBodyComponent>(other);

			float3 v1 = myBody.velocity * (myBody.mass - otherBody.mass);
			v1 += 2 * otherBody.mass * otherBody.velocity;
			v1 /= myBody.mass + otherBody.mass;

			float3 v2 = otherBody.velocity * (otherBody.mass - myBody.mass);
			v2 += 2 * myBody.mass * myBody.velocity;
			v2 /= myBody.mass + otherBody.mass;

			myBody.velocity = v1;
			otherBody.velocity = v2;
		};

		const ecs::Entity e1 = testScene->newEntity();
		float3 position{ -.875f, 0.f, 0.f };
		testScene->domain().addComponent<scene::components::TransformComponent>(
			e1,
			{
				position,
				quaternion(0.0f),
				float3(1)
		  }
		);
		testScene->domain().addComponent<scene::components::MeshComponent>(e1, { mesh, pipeline });
		testScene->domain().addComponent(e1,
			phy::RigidBodyComponent{
				1.f,
				{ 0.f, 0.f , 0.f},	{ 0.1f, 0.f, 0.f },
			}
		);
		testScene->domain().addComponent(e1,
			phy::ColliderComponent{
				.type = phy::aabb,
				.shape = phy::AABB (
						position,
						position + float3{ .25f, -.25f , 0.0f}
				),
				.action = ideallyElasticCollision
			}
		);


		const ecs::Entity e2 = testScene->newEntity();
		position = { .75f, 0.f, 0.f };
		testScene->domain().addComponent<scene::components::TransformComponent>(
			e2,
			{
				position,
				quaternion(0.0f),
				float3(1)
		  }
		);
		testScene->domain().addComponent<scene::components::MeshComponent>(e2, { mesh, pipeline });
		testScene->domain().addComponent(e2,
			phy::RigidBodyComponent{
				.mass = 5.f,
				.force = { 0.f, 0.f, 0.f },
				.velocity = { -0.1f, 0.f, 0.f } ,
			}
		);
		testScene->domain().addComponent(e2,
			phy::ColliderComponent{
				.type = phy::aabb,
				.shape = phy::AABB (
						position,
						position + float3{0.25f, -0.25f, 0.0f}
				),
				.action = ideallyElasticCollision
			}
		);

		scene::SceneManager::get()->changeScene(testScene);
		_physicsSystem = createRef<phy::System>(std::ref(testScene->domain()));
	}

	void update() override {
		_physicsSystem->update();
	}

private:
	Ref<phy::System> _physicsSystem;
};
} // namespace physics_example
