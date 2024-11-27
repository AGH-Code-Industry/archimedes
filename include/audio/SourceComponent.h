#pragma once
#include <string>
#include <AL/al.h>

namespace arch::audio{
	enum SourceState {
		ignored, playing, paused, stopped
	};

	struct SourceComponent{
		ALfloat pitch = 1.0f;
		ALfloat gain = 1.0f;
		ALfloat positionX = 0.0f, positionY = 0.0f;
		ALfloat velocityX = 0.0f, velocityY = 0.0f;
		bool isLooped = false;
		int sourceIndex = -1;
		std::string path = "";

		void play() {
			_state = playing;
		}
		void pause() {
			_state = paused;
		}
		void stop() {
			_state = stopped;
		}

		void ignore() {
			_state = ignored;
		}

		SourceState getState() const {
			return _state;
		}

		private:
			SourceState _state = ignored;
	};
}