#pragma once

#include <archimedes/Engine.h>
#include <archimedes/Input.h>
#include <archimedes/Scene.h>
#include <archimedes/physics/PhysicsSystem.h>
#include <archimedes/physics/collisions/ColliderComponent.h>
#include <archimedes/physics/components/RigidBodyComponent.h>

namespace physicsExample {
using namespace arch;
namespace phy = physics;


struct InteractiveCollisionTestApp final: Application {

	enum class PlayerColor {
		Red,
		Green,
		Blue
	};

	struct Vertex {
		float3 position;
		float2 tex_coords;
	};

	f32 windowWidth = 1'200.f;
	f32 windowHeight = 600.f;

	std::mutex mutex;
	ecs::Entity player;
	Ref<Scene> scene;

	f32 linearVelocityBase = 1.0f;
	f32 angularVelocityBase = glm::pi<f32>() / 4.0f;

	arch::Ref<arch::gfx::pipeline::Pipeline> pipelineRed, pipelineGreen, pipelineBlue, pipelineCircle;
	Ref<asset::mesh::Mesh> meshRectangle, meshTriangle;

	bool isCollidingMouse = false;
	bool isCollidingEntity = false;
	
	void createRectangleMesh() {

		std::vector<Vertex> vertices{
			{ { -0.25f, -0.25f, 0.1f }, { 0.f, 0.f } },
			{  { 0.25f, -0.25f, 0.1f }, { 1.f, 0.f } },
			{	  { 0.25f, 0.25f, 0.1f }, { 1.f, 1.f } },
			{  { -0.25f, 0.25f, 0.1f }, { 0.f, 1.f } },
		};

		std::vector<u32> indices{ 0, 3, 2, 2, 1, 0 };
		meshRectangle = asset::mesh::Mesh::create<Vertex>(vertices, indices);
	}

	
	void createTriangleMesh() {

		std::vector<Vertex> vertices{
			{ { -0.25f, -0.25f, 0.1f }, { 0.f, 0.f } },
			{  { 0.25f, -0.25f, 0.1f }, { 1.f, 0.f } },
			{	  { 0.0f, 0.25f, 0.1f }, { 0.5f, 1.f } },
		};

		std::vector<u32> indices{ 2, 1, 0 };
		meshTriangle = asset::mesh::Mesh::create<Vertex>(vertices, indices);
	}

	void createPipelines(const Ref<gfx::Renderer> renderer) {
		struct UniformBuffer {
			Mat4x4 projection;
		};

		UniformBuffer ubo{ glm::mat4{ 1 } };
		auto uniformBuffer =
			renderer->getBufferManager()->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));

		pipelineRed = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default2.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		});

		pipelineGreen = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default3.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		});

		pipelineBlue = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default4.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		});
		
		pipelineCircle = renderer->getPipelineManager()->create({
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default_circle.glsl",
			.textures = {},
			.buffers = { uniformBuffer },
		});
	}

	void setPlayerColor(ecs::Entity player, PlayerColor color) {
		switch (color) {
			case PlayerColor::Red:
				scene->domain().getComponent<scene::components::MeshComponent>(player).pipeline = pipelineRed;
				break;
			case PlayerColor::Green:
				scene->domain().getComponent<scene::components::MeshComponent>(player).pipeline = pipelineGreen;
				break;
			case PlayerColor::Blue:
				scene->domain().getComponent<scene::components::MeshComponent>(player).pipeline = pipelineBlue;
				break;
		}
	}

	ecs::Entity addPlayer() {
		auto player = scene->newEntity();
		float3 position{ 0.f, 0.f, 0.f };
		scene->domain()
			.addComponent<scene::components::TransformComponent>(player, { position, quaternion(0.0f), float3(1) });
		scene->domain().addComponent<scene::components::MeshComponent>(player, { meshRectangle, pipelineBlue });
		scene->domain().addComponent(
			player,
			phy::RigidBodyComponent{
				1.f,
				{ 0.f, 0.f, 0.f },
				{ 0.f, 0.f, 0.f },
		}
		);
		scene->domain().addComponent(
			player,
			phy::ColliderComponent{ .shape =
										phy::OBB(float3{ -0.25f, 0.25f, 0.0f }, float3{ 0.25f, -0.25f, 0.0f }, 0.0f),
									.detectsMouse = true, .isScannedMask = 0 }
		);
		return player;
	}

	void addRectangle() {
		const ecs::Entity e = scene->newEntity();
		float3 position = { 0.6f, 0.f, 0.f };
		scene->domain()
			.addComponent<scene::components::TransformComponent>(e, { position, quaternion(0.0f), float3(1) });
		scene->domain().addComponent<scene::components::MeshComponent>(e, { meshRectangle, pipelineBlue });
		scene->domain().addComponent(
			e,
			phy::ColliderComponent{
				.shape = phy::OBB(float3{ -0.25f, 0.25f, 0.0f }, float3{ 0.25f, -0.25f, 0.0f }, 0.0f),
				.scansMask = 0
			}
		);
	}

	void addTriangle() {
		const ecs::Entity e = scene->newEntity();
		float3 position = { -0.8f, -0.5f, 0.f };
		scene->domain()
			.addComponent<scene::components::TransformComponent>(e, { position, quaternion(0.0f), float3(1) });
		scene->domain().addComponent<scene::components::MeshComponent>(e, { meshTriangle, pipelineBlue });
		scene->domain().addComponent(
			e,
			phy::ColliderComponent{
				.shape = phy::Triangle(
					float3{ -0.25f, -0.25f, 0.0f },
					float3{ 0.25f, -0.25f, 0.0f },
					float3{ 0.0f, 0.25f, 0.0f }
				),
				.scansMask = 0
			}
		);
	}

	void addCircle() {
		const ecs::Entity e = scene->newEntity();
		float3 position = { 0.1f, 0.7f, 0.f };
		scene->domain()
			.addComponent<scene::components::TransformComponent>(e, { position, quaternion(0.0f), float3(1) });
		scene->domain().addComponent<scene::components::MeshComponent>(e, { meshRectangle, pipelineCircle });
		scene->domain().addComponent(
			e,
			phy::ColliderComponent{ .shape =
										phy::Circle(float3{ 0.0f, 0.0f, 0.0f },  0.25f),
									.scansMask = 0 }
		);
	}

	void init() override {
		scene = createRef<Scene>();
		const Ref<gfx::Renderer> renderer = gfx::Renderer::getCurrent();

		createPipelines(renderer);
		createRectangleMesh();
		createTriangleMesh();

		player = addPlayer();

		// a rectangle you can collide with
		addRectangle();

		// a triangle you can collide with
		addTriangle();

		// a circle you can collide with
		addCircle();

		scene::SceneManager::get()->changeScene(scene);
		_physicsSystem = createRef<phy::PhysicsSystem>(std::ref(scene->domain()), windowWidth, windowHeight);
	}

	float3 getLinearVelocity() {
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

	f32 getAngularVelocity() {
		f32 velocity{};
		if (Keyboard::Q.down()) {
			velocity -= 1.0f;
		}
		if (Keyboard::E.down()) {
			velocity += 1.0f;
		}
		return velocity;
	}

	void update() override {
		auto playerPosition = scene->domain().getComponent<scene::components::TransformComponent>(player).position;
		if(_physicsSystem->getEnteredCollisions(player).size() > 0) {
			if (!isCollidingMouse) {
				setPlayerColor(player, PlayerColor::Green);
			}
			isCollidingEntity = true;
			Logger::info("Collision detected! Position: {}, {}", playerPosition.x, playerPosition.y);
		}
		if(_physicsSystem->getExitedCollisions(player).size() > 0) {
			if (!isCollidingMouse) {
				setPlayerColor(player, PlayerColor::Blue);
			}
			isCollidingEntity = false;
			Logger::info("Collision ended! Position: {}, {}", playerPosition.x, playerPosition.y);
		}
		float3 mousePosition = _physicsSystem->getMousePositionOnMap();
		if(_physicsSystem->hasMouseEntered(player)) {
			setPlayerColor(player, PlayerColor::Red);
			isCollidingMouse = true;
			Logger::info("Mouse entered! Position: {}, {}", mousePosition.x, mousePosition.y);
		}
		if(_physicsSystem->hasMouseExited(player)) {
			if (isCollidingEntity) {
				setPlayerColor(player, PlayerColor::Green);
			} else {
				setPlayerColor(player, PlayerColor::Blue);
			}
			isCollidingMouse = false;
			Logger::info("Mouse exited! Position: {}, {}", mousePosition.x, mousePosition.y);
		}
		const float3 linearVelocity = linearVelocityBase * getLinearVelocity();
		const f32 angularVelocity = angularVelocityBase * getAngularVelocity();
		auto& rigidBody = scene->domain().getComponent<phy::RigidBodyComponent>(player);
		rigidBody.linearVelocity = linearVelocity;
		rigidBody.angularVelocity = angularVelocity;
		_physicsSystem->update();
	}

private:
	Ref<phy::PhysicsSystem> _physicsSystem;
};
} // namespace physics_example
