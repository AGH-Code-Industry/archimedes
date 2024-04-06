#include <Logger.h>
#include <audio/calls.hpp>

#include <audio/exception.h>
#include <audio/sound_device.h>

namespace arch::audio {
SoundDevice::SoundDevice() {
	alcDevice = alcOpenDevice(nullptr);
	if(not alcDevice){
		throw AudioException("Failed to open device");
	} 

	alcContext = alcCall(alcCreateContext, alcDevice, alcDevice, nullptr);

	alcCall(alcMakeContextCurrent, alcDevice, alcContext);

	arch::Logger::info("Audio system: opened device");
}
SoundDevice::~SoundDevice() {
	alcCall(alcMakeContextCurrent, alcDevice, nullptr);
	alcCall(alcCloseDevice, alcDevice, alcDevice);
	arch::Logger::info("Audio system: closed device");
}
}  // namespace arch::audio