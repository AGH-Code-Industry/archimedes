#include <Logger.h>
#include <audio/Calls.hpp>

#include <audio/AudioException.h>
#include <audio/SoundDevice.h>

#include <iostream>

namespace arch::audio {


SoundDevice::SoundDevice() {
	alcDevice = alcOpenDevice(nullptr);
	if(!alcDevice) {
		throw AudioException("Failed to open device");
	}

	alcContext = alcCall(alcCreateContext, alcDevice, alcDevice, nullptr);

	alcCall(alcMakeContextCurrent, alcDevice, alcContext);

	Logger::info("Audio system: opened device");
}
SoundDevice::~SoundDevice() {
	alcCall(alcMakeContextCurrent, alcDevice, nullptr);
	alcCall(alcCloseDevice, alcDevice, alcDevice);
	Logger::info("Audio system: closed device");
}
}  // namespace arch::audio