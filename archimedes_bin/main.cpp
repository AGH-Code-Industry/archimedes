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



void testAudioSystem(ecs::Domain& domain) {
	// initialize OpenAL context
	audio::SoundDevice device;

	// initialize SoundBank
	audio::SoundBank soundBank;

	// load audio files that we can play
	soundBank.addClip(sounds + "Chiptone A4.wav");
	soundBank.loadInitialGroups();

	// initialize and start the audioManager
	audio::SourceManager audioManager(&soundBank, &domain);
	std::jthread audioThread(&audio::SourceManager::play, &audioManager);

	while(getchar() != 'q') {
		arch::Logger::debug("play");
		// create an entity and give it an audio source component
		auto wind = domain.newEntity();
		audio::SourceComponent& source = domain.addComponent<audio::SourceComponent>(wind);
		arch::Logger::debug("add");

		// set some custom parameters to the source
		source.path = sounds + "Chiptone A4.wav"; //this is mandatory, so we can play the sound
		source.gain = 0.5; // this is optional, it will make the sound quieter

		// play the source
		source.play();

	}

	//close the audioManager
	audioManager.stop();
}

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::EngineConfig config {
		600, 480, "Archimedes Test", glm::vec4(0, 0, 0, 0)
	};

	ecs::Domain domain;

	testAudioSystem(domain);
	// arch::Ref<MyApp> myApp = arch::createRef<MyApp>();
	//
	// arch::Engine engine { config, myApp };
	// engine.start();
}