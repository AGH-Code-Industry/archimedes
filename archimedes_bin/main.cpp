#include <iostream>
#include <numbers>
#include <random>

#include "scene/Components.h"
#include "scene/SceneManager.h"
#include <Engine.h>
#include <Logger.h>
#include <Net.h>
#include <audio/SoundDevice.h>
#include <audio/SourceManager.h>

struct VelocityComponent {
	arch::math::float3 velocity;
};

struct Gravity {
	arch::float3 value = { 0, -9.81f / 20'000, 0 };
};

struct ExplosionParticle {
	float rotationSpeed;
};

class Explosion {
public:

	Explosion(): _sock{ 30'420 } {}

	void deleteInvisibleParticles(arch::scene::Scene& scene) {
		for (auto&& [entity, _, transform] :
			 scene.domain().view<ExplosionParticle, arch::scene::components::TransformComponent>().all()) {
			if (transform.position.y < -0.75) {
				arch::Logger::debug(
					"deleted particle: [{}|{}]",
					arch::ecs::_details::EntityTraits::Id::part(entity),
					arch::ecs::_details::EntityTraits::Version::part(entity)
				);
				scene.removeEntity(entity);
			}
		}
	}

	void tryPerform(arch::scene::Scene& scene, arch::audio::SourceComponent& sc) {
		if (_sock.dataAvalible()) {
			char buf[8 + 1]{};
			_sock.recv(buf, 8);

			if (std::string_view(buf) == "BOOM!") {
				perform(scene, sc);
			}
		}
	}

	void perform(arch::scene::Scene& scene, arch::audio::SourceComponent& source) {
		auto& domain = scene.domain();

		auto rng = std::mt19937(std::random_device{}());

		auto angleLimit = std::numbers::pi_v<float> / 4;
		auto angleDistribution = std::uniform_real_distribution(-angleLimit, angleLimit);
		auto rotationSpeedDistribution = std::uniform_real_distribution(-20.0f, 20.0f);
		auto forceDistribution = std::uniform_real_distribution(0.75f, 1.25f);
		auto particleCount = std::uniform_int_distribution(10, 25)(rng);

		for (int i = 0; i != particleCount; ++i) {
			auto e = scene.newEntity();

			domain.addComponent<ExplosionParticle>(e, rotationSpeedDistribution(rng));
			domain.addComponent<arch::scene::components::MeshComponent>(e);
			domain.addComponent<Gravity>(e);
			float angle = angleDistribution(rng);
			domain.addComponent<VelocityComponent>(
				e,
				forceDistribution(rng) * 0.015f * arch::float3{ std::sin(angle), std::cos(angle), 0 }
			);
			domain.addComponent<arch::scene::components::TransformComponent>(
				e,
				arch::scene::components::TransformComponent{
					.position = { 0, 0.225, 0 },
					.rotation = arch::Quat{ 0, 0, 0, 1 },
					.scale = { 0.1, 0.1, 0 }
			 }
			);
		}

		if (source.getState() == arch::audio::SourceState::playing) {
			source.stop();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		source.play();
	}

private:

	arch::net::UDPSocket _sock;
};

struct MyApp: arch::Application {
	arch::Ref<arch::audio::SoundDevice> soundDevice;
	arch::Ref<arch::audio::SoundBank> soundBank;
	arch::Ref<arch::audio::SourceManager> audioManager;
	arch::Ref<std::jthread> audioManagerThread;

	void init() override {
		arch::Ref<arch::scene::Scene> testScene = arch::createRef<arch::scene::Scene>();
		auto&& testDomain = testScene->domain();

		soundDevice = arch::createRef<arch::audio::SoundDevice>();
		soundBank = arch::createRef<arch::audio::SoundBank>();

		soundBank->addClip("Explosion1.ogg");
		soundBank->loadInitialGroups();

		audioManager = arch::createRef<arch::audio::SourceManager>(soundBank.get(), &testDomain);
		audioManagerThread = arch::createRef<std::jthread>(&arch::audio::SourceManager::play, audioManager.get());

		{ // volcano entity
			auto volcano = testScene->newEntity();

			testDomain.addComponent<arch::scene::components::MeshComponent>(volcano);
			testDomain.addComponent<arch::scene::components::TransformComponent>(
				volcano,
				arch::scene::components::TransformComponent{
					.position = {},
					.rotation = arch::Quat{ 0, 0, 0, 1 },
					.scale = { 1, 1, 0 }
			 }
			);
			testDomain.addComponent<Explosion>(volcano);

			audioManager->execute([&]() {
				auto&& source = testDomain.addComponent<arch::audio::SourceComponent>(volcano);
				source.gain = 0.5;
				source.path = "Explosion1.ogg";
				source.removeOnStop = false;
			});
		}

		arch::scene::SceneManager::get()->changeScene(testScene);
	}

	void update() override {
		auto& scene = *arch::scene::SceneManager::get()->currentScene();
		auto& domain = scene.domain();

		auto rotateQuat = [](arch::Quat& quat, const arch::float3 axis, float angle) {
			auto axisNormalized = glm::normalize(axis);

			float halfAngle = glm::radians(angle) / 2.0f;
			float sinHalfAngle = glm::sin(halfAngle);
			glm::quat rotationQuat(
				glm::cos(halfAngle),
				axisNormalized.x * sinHalfAngle,
				axisNormalized.y * sinHalfAngle,
				axisNormalized.z * sinHalfAngle
			);

			quat = rotationQuat * quat;
		};

		domain.view<Explosion, arch::audio::SourceComponent>().forEach(
			[&scene](Explosion& explosion, arch::audio::SourceComponent& source) {
				explosion.deleteInvisibleParticles(scene);
				explosion.tryPerform(scene, source);
			}
		);

		for (auto&& [entity, gravity, velocity, transform] :
			 domain.view<Gravity, VelocityComponent, arch::scene::components::TransformComponent>().all()) {
			velocity.velocity += gravity.value;

			transform.position += velocity.velocity;

			auto particleOpt = domain.tryGetComponent<ExplosionParticle>(entity);
			if (particleOpt) {
				rotateQuat(transform.rotation, { 0, 0, 1 }, particleOpt->get().rotationSpeed);
			}
		}
	}
};

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Ref<MyApp> myApp = arch::createRef<MyApp>();

	arch::EngineConfig config{ .windowWidth = 900,
							   .windowHeight = 900,
							   .windowTitle = "Archimedes Example",
							   .backgroundColor = arch::Color(.01f, .01f, .01f, 1.f),
							   .renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK };

	arch::Engine engine{ config, myApp };
	engine.start();
}
