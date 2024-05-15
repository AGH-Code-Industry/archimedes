#include <Engine.h>
#include <Logger.h>
#include <audio/SoundDevice.h>
#include <audio/AudioSource.h>
#include <thread>
#include <iostream>

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Logger::trace("Hello, {}!", "World");
	arch::Logger::debug("Hello, {}!", "World");
	arch::Logger::info("Hello, {}!", "World");
	arch::Logger::warn("Hello, {}!", "World");
	arch::Logger::error("Hello, {}!", "World");
	arch::Logger::critical("Hello, {}!", "World");

	arch::Logger::log(arch::LogLevel::info, "Info log");

	arch::EngineConfig config {
		600, 480, "Archimedes Test", glm::vec4(0, 0, 0, 0)
	};


	arch::Engine engine { config };

	std::string cwd = "/home/anon/dev/archimedes-audio-system/archimedes/archimedes_bin/";
	arch::audio::SoundDevice device;
	arch::audio::AudioSource source(cwd + "rickroll.wav");
	std::thread audioThread(&arch::audio::AudioSource::play, &source);

	int isPausing;
	while(std::cin >> isPausing) {
		if(isPausing == 1) {
			source.isPaused = true;
		}
		else {
			source.isPaused = false;
		}
	}
}