#pragma once
#include <AL/al.h>
#include <AL/alc.h>

namespace arch::audio {
class SoundDevice {
public:
	SoundDevice();
	~SoundDevice();

	ALCdevice* alcDevice;
	ALCcontext* alcContext;
};

} // namespace arch::audio
