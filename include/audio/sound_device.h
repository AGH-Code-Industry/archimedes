#pragma once
#include <AL/alc.h>

namespace arch::audio {
class SoundDevice {
public:
	SoundDevice();
	~SoundDevice();

	ALCdevice* ALCDevice;
	ALCcontext* ALCContext;
};

}  // namespace arch::audio