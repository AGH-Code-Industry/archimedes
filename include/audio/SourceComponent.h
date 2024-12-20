#pragma once
#include <string>

#include <AL/al.h>
#include <Logger.h>

namespace arch::audio{
	enum SourceState {
		ignoring, playing, pausing, stopping
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
			if(_state == pausing || _state == ignoring) {
				_state = playing;
			}
		}
		void pause() {
			if(_state == playing) {
				_state = pausing;
			}
		}
		void stop() {
			if(_state == playing || _state == pausing) {
				_state = stopping;
			}
		}

		void ignore() {
			if(_state == stopping) {
				_state = ignoring;
			}
		}

		SourceState getState() const {
			return _state;
		}

		private:
			SourceState _state = ignoring;
	};
}