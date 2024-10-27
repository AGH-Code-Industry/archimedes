#include <Engine.h>
#include <Logger.h>
#include <audio/SoundDevice.h>
#include <audio/SourceManager.h>
#include <thread>
#include <cmath>

const std::string sounds = "/home/anon/dev/archimedes/archimedes_bin/sounds/";

struct MyApp : arch::Application {
	void init() override {
		arch::Logger::info("Initializing user app!");
	}
};

void testGainChange(arch::audio::SourceManager& audioManager) {
	audioManager.addSource(sounds + "wind.mp3", 1.0f, 1.0f);
	audioManager.addSource(sounds + "rickroll.wav", 1.0f, 0.1f);

	std::jthread audioThread(&arch::audio::SourceManager::play, &audioManager);
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

void testDistanceChange(arch::audio::SourceManager& audioManager) {
	audioManager.addSource(sounds + "wind.mp3", 1.0f, 1.0f);
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
			audioManager.pauseSource(0);
		}
		else if(i == 20) {
			audioManager.continueSource(0);
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

	arch::audio::SourceManager audioManager(&soundBank);

	testDistanceChange(audioManager);

	// arch::Ref<MyApp> myApp = arch::createRef<MyApp>();
	//
	// arch::Engine engine { config, myApp };
	// engine.start();
}