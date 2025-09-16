#pragma once

#include <archimedes/physics/RigidBodyComponent.h>
#include <archimedes/Engine.h>
#include <archimedes/Scene.h>
#include <archimedes/physics/ColliderComponent.h>
#include <archimedes/physics/System.h>
#include <archimedes/Input.h>


namespace physicsExample {
using namespace arch;
namespace phy = physics;


struct InteractiveCollisionTestApp final: Application {

	std::mutex mutex;
	ecs::Entity player;
	Ref<Scene> scene;

	float velocityBase = 1.0f;

	void init() override {
		scene = createRef<Scene>();

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

		auto collision = [&, &domain = scene->domain()](const ecs::Entity me, const ecs::Entity other) {
			auto position = domain.getComponent<scene::components::TransformComponent>(me).position;
			Logger::info("Collision spotted at: ({}, {}, {})", position.x, position.y, position.z);
		};

		player = scene->newEntity();
		float3 position{ 0.f, 0.f, 0.f };
		scene->domain().addComponent<scene::components::TransformComponent>(
			player,
			{
				position,
				quaternion(0.0f),
				float3(1)
		  }
		);
		scene->domain().addComponent<scene::components::MeshComponent>(player, { mesh, pipeline });
		scene->domain().addComponent(player,
			phy::RigidBodyComponent{
				1.f,
				{ 0.f, 0.f , 0.f},	{ 0.f, 0.f, 0.f },
			}
		);
		scene->domain().addComponent(player,
			phy::ColliderComponent{
				.type = phy::aabb,
				.shape = phy::AABB (
						position,
						position + float3{ 0.25f, -0.25f , 0.0f}
				),
				.action = collision,
			}
		);


		const ecs::Entity e2 = scene->newEntity();
		position = { 0.75f, 0.f, 0.f };
		scene->domain().addComponent<scene::components::TransformComponent>(
			e2,
			{
				position,
				quaternion(0.0f),
				float3(1)
		  }
		);
		scene->domain().addComponent<scene::components::MeshComponent>(e2, { mesh, pipeline });
		scene->domain().addComponent(e2,
			phy::RigidBodyComponent{
				.mass = 5.f,
				.force = { 0.f, 0.f, 0.f },
				.velocity = { 0.f, 0.f, 0.f } ,
			}
		);
		scene->domain().addComponent(e2,
			phy::ColliderComponent{
				.type = phy::aabb,
				.shape = phy::AABB (
						position,
						position + float3{0.25f, -0.25f, 0.0f}
				),
				.action = collision,
			}
		);

		scene::SceneManager::get()->changeScene(scene);
		_physicsSystem = createRef<phy::System>(std::ref(scene->domain()));
	}

	float3 getVelocity() {
		auto lock = std::lock_guard(mutex);
		float3 velocity{};
		if (Keyboard::W.down()) {
			velocity += float3{0.0f, 1.0f, 0.0f};
		}
		if (Keyboard::S.down()) {
			velocity -= float3{0.0f, 1.0f, 0.0f};
		}
		if (Keyboard::A.down()) {
			velocity -= float3{1.0f, 0.0f, 0.0f};
		}
		if (Keyboard::D.down()) {
			velocity += float3{1.0f, 0.0f, 0.0f};
		}
		return velocity;
	}

	void update() override {
		const float3 velocity = velocityBase * getVelocity();
		scene->domain().getComponent<phy::RigidBodyComponent>(player).velocity = velocity;
		_physicsSystem->update();
	}

private:
	Ref<phy::System> _physicsSystem;
};
} // namespace physics_example
