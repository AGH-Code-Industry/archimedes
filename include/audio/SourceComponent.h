#pragma once
#include <string>
#include <AL/al.h>

namespace arch::audio{
	struct SourceComponent{
		ALfloat pitch;
		ALfloat gain;
		ALfloat positionX, positionY;
		ALfloat velocityX, velocityY;
		bool isLooped;
		int sourceIndex = -1;
		std::string path;
	};
}