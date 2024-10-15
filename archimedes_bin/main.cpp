#include <Engine.h>
#include <Logger.h>
#include <audio/SoundDevice.h>
#include <audio/AudioSource.h>
#include <audio/AudioManager.h>
#include <thread>
#include <unistd.h>


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
	soundBank.loadInitialGroups();

	arch::audio::AudioManager audioManager(&soundBank);
	audioManager.addSource(sounds + "wind.mp3");

	//arch::audio::AudioSource source(soundBank, cwd + "rickroll.wav");
	//std::thread audioThread(&arch::audio::AudioSource::play, &source);
	std::thread audioThread(&arch::audio::AudioManager::play, &audioManager);

	int gainModifier = 0;
	for(int i=0; i < 5; i++) {
		gainModifier += 4;
		ALfloat pitch = gainModifier / 10.0f;
		audioManager.changePitch(0, pitch);
		arch::Logger::debug("pitch: {}", pitch);
		sleep(5);
	}
	audioManager.isListening = false;

	// arch::Ref<MyApp> myApp = arch::createRef<MyApp>();
	//
	// arch::Engine engine { config, myApp };
	// engine.start();
}