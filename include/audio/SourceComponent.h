#pragma once
#include <string>
#include <AL/al.h>

namespace arch::audio{
	struct SourceComponent{
		ALfloat pitch = 1.0f;
		ALfloat gain = 1.0f;
		ALfloat positionX = 0.0f, positionY = 0.0f;
		ALfloat velocityX = 0.0f, velocityY = 0.0f;
		bool isLooped = false;
		int sourceIndex = -1;
		std::string path = "";
	};
}