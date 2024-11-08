#include <Engine.h>
#include <Logger.h>
#include <audio/SoundDevice.h>
#include <audio/SourceManager.h>
#include <thread>
#include <cmath>
#include <Ecs.h>

const std::string sounds = "/home/anon/dev/archimedes/archimedes_bin/sounds/";

namespace audio = arch::audio;
namespace ecs = arch::ecs;

struct MyApp : arch::Application {
	void init() override {
		arch::Logger::info("Initializing user app!");
	}
};

void testGainChange(audio::SourceManager& audioManager, ecs::Domain& domain) {
	auto windPlayer = domain.newEntity();
	auto rickrollPlayer = domain.newEntity();
	domain.addComponent<audio::SourceComponent>(windPlayer);
	domain.addComponent<audio::SourceComponent>(rickrollPlayer);
	audio::SourceComponent& wind = domain.getComponent<audio::SourceComponent>(windPlayer);
	audio::SourceComponent& rickroll = domain.getComponent<audio::SourceComponent>(rickrollPlayer);
	wind.path = sounds + "wind.mp3";
	rickroll.path = sounds + "rickroll.wav";
	rickroll.gain = 0.1f;

	audioManager.addSource(wind);
	audioManager.addSource(rickroll);

	std::jthread audioThread(&audio::SourceManager::play, &audioManager);
	int gainModifier = 0;
	for(int i=0; i < 5; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		gainModifier += 2;
		ALfloat gain = gainModifier / 10.0f;
		wind.gain = 1.0f - gain;
		rickroll.gain = gain;
	}

	audioManager.isListening = false;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	sleep(2);
}

void testDistanceChange(audio::SourceManager& audioManager, ecs::Domain& domain) {
	auto windPlayer = domain.newEntity();
	domain.addComponent<audio::SourceComponent>(windPlayer);
	audio::SourceComponent& wind = domain.getComponent<audio::SourceComponent>(windPlayer);
	wind.path = sounds + "wind.mp3";
	wind.positionX = 1.0f;
	audioManager.addSource(wind);
	std::jthread audioThread(&arch::audio::SourceManager::play, &audioManager);
	float radius = 1.0f;
	int steps = 30;
	for(int i=0; i < steps; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		float x = radius * std::cos(i * 6.28 / steps);
		float y = radius * std::sin(i * 6.28 / steps);
		wind.positionX = x;
		wind.positionY = y;
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

	audio::SoundDevice device;
	audio::SoundBank soundBank;
	soundBank.addClip(sounds + "wind.mp3");
	soundBank.addClip(sounds + "rickroll.wav");
	soundBank.loadInitialGroups();

	ecs::Domain domain;
	audio::SourceManager audioManager(&soundBank, &domain);

	testDistanceChange(audioManager, domain);

	// arch::Ref<MyApp> myApp = arch::createRef<MyApp>();
	//
	// arch::Engine engine { config, myApp };
	// engine.start();
}