#include <Engine.h>
#include <Logger.h>
#include <audio/SoundDevice.h>
#include <audio/AudioManager.h>
#include <thread>
#include <cmath>
#include <Ecs.h>
#include <mutex>

const std::string sounds = "/home/anon/dev/archimedes/archimedes_bin/sounds/";

namespace audio = arch::audio;
namespace ecs = arch::ecs;

struct MyApp : arch::Application {
	void init() override {
		arch::Logger::info("Initializing user app!");
	}
};

audio::AudioSource* createSource(std::mutex& mutex, ecs::Domain& domain,
									const std::string& name, bool isLooped) {
	// create an entity and give it an audio source component
	auto lock = std::lock_guard(mutex);
	auto entity = domain.newEntity();
	auto source = &domain.addComponent<audio::AudioSource>(entity);

	// set some custom parameters to the source
	source->path = sounds + name; //this is mandatory, so we can play the sound
	source->gain = 0.5; // this is optional, it will make the sound quieter
	source->isLooped = isLooped;  // this will make the sound looped, so it won`t be removed automatically

	return source;
}


void testSimpleSound(ecs::Domain& domain) {
	// initialize OpenAL context
	audio::SoundDevice device;

	// initialize SoundBank
	audio::SoundBank soundBank;

	// audio file name
	const std::string filename = "Chiptone A4.wav";

	// load audio files that we can play
	soundBank.addClip(sounds + filename);
	soundBank.loadInitialGroups();

	// initialize and start the audioManager
	std::mutex mutex;
	audio::AudioManager audioManager(&soundBank, &domain, mutex);
	std::jthread audioThread(&audio::AudioManager::play, &audioManager);

	while(getchar() != 'q') {
		// create an entity and give it an audio source component
		auto source = createSource(mutex, domain, filename, false);

		// play the source
		source->play();
	}

	//close the audioManager
	audioManager.stop();
}

void testControl(ecs::Domain& domain) {
	// initialize OpenAL context
	audio::SoundDevice device;

	// initialize SoundBank
	audio::SoundBank soundBank;

	// audio file name
	const std::string filename = "rickroll.wav";

	// load audio files that we can play
	soundBank.addClip(sounds + filename);
	soundBank.loadInitialGroups();

	// initialize and start the audioManager
	std::mutex mutex;
	audio::AudioManager audioManager(&soundBank, &domain, mutex);
	std::jthread audioThread(&audio::AudioManager::play, &audioManager);

	auto source = createSource(mutex, domain, filename, true);
	source->play();

	char controlSign = 'x';
	bool isPlaying = true;
	while(controlSign != 'q') {
		controlSign = getchar();
		switch(controlSign) {
			case 'p':
				source->pause();
				break;
			case 'c':
				source->play();
				break;
			case 's':
				source->stop();
				isPlaying = false;
				break;
			case 'r':
				if(!isPlaying) {
					source = createSource(mutex, domain, filename, true);
					source->play();
				}
				break;
			default:
				break;
		}
	}

	//close the audioManager
	audioManager.stop();
}

void testSpatialAudio(ecs::Domain& domain) {
	// initialize OpenAL context
	audio::SoundDevice device;

	// initialize SoundBank
	audio::SoundBank soundBank;

	// audio file name
	const std::string filename = "wind.mp3";

	// load audio files that we can play
	soundBank.addClip(sounds + filename);
	soundBank.loadInitialGroups();

	// initialize and start the audioManager
	std::mutex mutex;
	audio::AudioManager audioManager(&soundBank, &domain, mutex);
	std::jthread audioThread(&audio::AudioManager::play, &audioManager);

	// create an entity and give it an audio source component
	auto source = createSource(mutex, domain, filename, true);

	{
		auto lock = std::lock_guard(mutex);
		source->positionX = 0.0f;
		source->positionY = 1.0f;
	}

	// play the source
	source->play();

	// during playing the sound, update the source every 50 ms.
	// make the source hang around the listener while changing the distance from them.
	const int steps = 1000;
	for (int i=0; i<steps; i++) {
		const int stepsPerCircle = 100;
		const float distance = 5.0f + 5.0 * std::sin(i * 2 * std::numbers::pi / stepsPerCircle);
		{
			auto lock = std::lock_guard(mutex);
			source->positionX = distance * std::cos(i * 2 * std::numbers::pi / stepsPerCircle);
			source->positionY = distance * std::sin(i * 2 * std::numbers::pi / stepsPerCircle);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
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

	// testSimpleSound(domain);
	// testControl(domain);
	testSpatialAudio(domain);

	// arch::Ref<MyApp> myApp = arch::createRef<MyApp>();
	//
	// arch::Engine engine { config, myApp };
	// engine.start();
}