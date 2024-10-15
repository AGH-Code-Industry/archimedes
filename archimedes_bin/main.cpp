#include <Engine.h>
#include <Logger.h>
#include <audio/SoundDevice.h>
#include <audio/AudioSource.h>
#include <audio/AudioManager.h>
#include <thread>
#include <iostream>

struct MyApp : arch::Application {
	void init() override {
		arch::Logger::info("Initializing user app!");
	}
};

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::EngineConfig config {
		600, 480, "Archimedes Test", glm::vec4(0, 0, 0, 0)
	};

	std::string sounds = "/home/anon/dev/archimedes/archimedes_bin/sounds";
	arch::audio::SoundDevice device;
	arch::audio::SoundBank soundBank;
	soundBank.addClip(sounds + "rickroll.wav");
	soundBank.addClip(sounds + "moonlight.mp3");
	soundBank.loadInitialGroups();

	arch::audio::AudioManager audioManager(&soundBank);
	audioManager.addSource(sounds + "moonlight.mp3");
	audioManager.addSource(sounds + "rickroll.wav");

	//arch::audio::AudioSource source(soundBank, cwd + "rickroll.wav");
	//std::thread audioThread(&arch::audio::AudioSource::play, &source);
	std::thread audioThread(&arch::audio::AudioManager::play, &audioManager);

	
	int isPausing;
	while(std::cin >> isPausing) {
		if(isPausing == 1) {
			//audioManager.isListening = false;
			audioManager.removeSource(0);
		}
	}

	arch::Ref<MyApp> myApp = arch::createRef<MyApp>();

	arch::Engine engine { config, myApp };
	engine.start();
}