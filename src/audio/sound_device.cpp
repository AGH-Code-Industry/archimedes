#include <Logger.h>
#include <audio/exception.h>
#include <audio/sound_device.h>

namespace arch::audio {
SoundDevice::SoundDevice() {
	ALCDevice = alcOpenDevice(nullptr);
	if (!ALCDevice) {
		throw AudioException("Failed to get sound device");
	}

	ALCContext = alcCreateContext(ALCDevice, nullptr);
	if (!ALCContext) {
		throw AudioException("Failed to set sound context");
	}

	if (!alcMakeContextCurrent(ALCContext))
		throw AudioException("Failed to make context current");

	const ALCchar* name = nullptr;

	if (alcIsExtensionPresent(ALCDevice, "ALC_ENUMERATE_ALL_EXT")) {
		name = alcGetString(ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	}
	if (not name or alcGetError(ALCDevice) != ALC_NO_ERROR) {
		name = alcGetString(ALCDevice, ALC_DEVICE_SPECIFIER);
	}

	arch::Logger::info("Audio system: opened device {}", name);
}
SoundDevice::~SoundDevice() {
	// if (!alcMakeContextCurrent(nullptr))
	// 	throw AudioException("Failed to set context to nullptr");

	// alcDestroyContext(ALCContext);

	// if (ALCContext) throw AudioException("Failed to unset during close");

	// if (!alcCloseDevice(ALCDevice))
	// 	throw AudioException("Failed to close sound device");

	// arch::Logger::info("Audio system: closed device");
}
}  // namespace arch::audio