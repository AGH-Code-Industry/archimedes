#include <Engine.h>
#include <Logger.h>
#include <audio/SoundDevice.h>
#include <audio/SourceManager.h>
#include <thread>
#include <cmath>
#include <ecs/Domain.h>

const std::string sounds = "/home/anon/dev/archimedes/archimedes_bin/sounds/";

namespace audio = arch::audio;
namespace ecs = arch::ecs;

struct MyApp : arch::Application {
	void init() override {
		arch::Logger::info("Initializing user app!");
	}
};

void testGainChange(audio::SourceManager& audioManager, ecs::Domain<ecs::e64>& domain) {
	ecs::e64 wind = domain.newEntity();
	ecs::e64 rickroll = domain.newEntity();
	domain.addComponent<audio::SourceComponent>(wind);
	domain.addComponent<audio::SourceComponent>(rickroll);
	domain.getComponent<audio::SourceComponent>(wind).path = sounds + "wind.mp3";
	domain.getComponent<audio::SourceComponent>(rickroll).path = sounds + "rickroll.wav";
	domain.getComponent<audio::SourceComponent>(rickroll).gain = 0.1f;


	audioManager.addSource(domain.getComponent<audio::SourceComponent>(wind));
	audioManager.addSource(domain.getComponent<audio::SourceComponent>(rickroll));

	std::jthread audioThread(&audio::SourceManager::play, &audioManager);
	int gainModifier = 0;
	for(int i=0; i < 5; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		gainModifier += 2;
		ALfloat gain = gainModifier / 10.0f;
		audioManager.mixer.changeSourceGain(0, 1.0f - gain);
		audioManager.mixer.changeSourceGain(1, gain);
	}

	audioManager.isListening = false;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	sleep(2);
}

void testDistanceChange(arch::audio::SourceManager& audioManager, arch::ecs::Domain<arch::ecs::e64>& domain) {
	arch::audio::SourceComponent wind;
	wind.path = sounds + "wind.mp3";
	audioManager.addSource(wind);
	std::jthread audioThread(&arch::audio::SourceManager::play, &audioManager);
	float radius = 1.0f;
	audioManager.mixer.changeSourcePosition(0, 1.0f, 0.0f);
	int steps = 30;
	for(int i=0; i < steps; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		float x = radius * std::cos(i * 6.28 / steps);
		float y = radius * std::sin(i * 6.28 / steps);
		audioManager.mixer.changeSourcePosition(0, x, y);
		if(i == 10) {
			audioManager.pauseSource(wind);
		}
		else if(i == 20) {
			audioManager.continueSource(wind);
		}
	}
	audioManager.isListening = false;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	sleep(2);
}

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::EngineConfig config {
		600, 480, "Archimedes Test", glm::vec4(0, 0, 0, 0)
	};

	arch::audio::SoundDevice device;
	arch::audio::SoundBank soundBank;
	soundBank.addClip(sounds + "wind.mp3");
	soundBank.addClip(sounds + "rickroll.wav");
	soundBank.loadInitialGroups();

	ecs::Domain<ecs::e64> domain;
	arch::audio::SourceManager audioManager(&soundBank, &domain);

	testGainChange(audioManager);

	// arch::Ref<MyApp> myApp = arch::createRef<MyApp>();
	//
	// arch::Engine engine { config, myApp };
	// engine.start();
}