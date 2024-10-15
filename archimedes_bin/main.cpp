#include <Engine.h>
#include <Logger.h>
#include <audio/SoundDevice.h>
#include <audio/AudioSource.h>
#include <audio/AudioManager.h>
#include <thread>
#include <unistd.h>
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

	std::string sounds = "/home/anon/dev/archimedes/archimedes_bin/sounds/";
	arch::audio::SoundDevice device;
	arch::audio::SoundBank soundBank;
	soundBank.addClip(sounds + "wind.mp3");
	soundBank.addClip(sounds + "rickroll.wav");
	soundBank.loadInitialGroups();

	arch::audio::AudioManager audioManager(&soundBank);
	audioManager.addSource(sounds + "wind.mp3", 1.0f, 1.0f);
	audioManager.addSource(sounds + "rickroll.wav", 1.0f, 0.1f);

	std::thread audioThread(&arch::audio::AudioManager::play, &audioManager);

	int gainModifier = 0;
	for(int i=0; i < 5; i++) {
		sleep(3);
		gainModifier += 2;
		ALfloat gain = gainModifier / 10.0f;
		arch::audio::AudioSource* windSource = &audioManager.audioSources[1];
		arch::audio::AudioSource* rickrollSource = &audioManager.audioSources[1];
		std::mutex* mutex = &audioManager.mutex;
		windSource->changeGain(1.0f - gain, mutex);
		rickrollSource->changeGain(gain, mutex);
	}

    audioManager.isListening = false;
	sleep(2);



	// arch::Ref<MyApp> myApp = arch::createRef<MyApp>();
	//
	// arch::Engine engine { config, myApp };
	// engine.start();
}