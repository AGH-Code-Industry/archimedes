#include <audio/Calls.hpp>

#include <Logger.h>
#include <audio/AudioException.h>
#include <audio/SoundDevice.h>

namespace arch::audio {

SoundDevice::SoundDevice() {
	ALCdevice* alcDevice = alcOpenDevice(nullptr);
	if (!alcDevice) {
		throw AudioException("Failed to open device");
	}
	ALCcontext* alcContext = alcCall(alcCreateContext, alcDevice, alcDevice, nullptr);
	alcCall(alcMakeContextCurrent, alcDevice, alcContext);
	device = reinterpret_cast<char*>(alcDevice);
	context = reinterpret_cast<char*>(alcContext);
	Logger::info("Audio system: opened device");
}

SoundDevice::~SoundDevice() {
	ALCdevice* alcDevice = reinterpret_cast<ALCdevice*>(device);
	alcCall(alcMakeContextCurrent, alcDevice, nullptr);
	alcCall(alcCloseDevice, alcDevice, alcDevice);
	Logger::info("Audio system: closed device");
}
} // namespace arch::audio
