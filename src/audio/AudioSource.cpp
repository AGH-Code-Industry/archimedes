#include <audio/AudioSource.h>

namespace arch::audio {
void AudioSource::play() {
	if(_state == pausing || _state == ignoring) {
		_state = playing;
	}
}

void AudioSource::pause() {
	if(_state == playing) {
		_state = pausing;
	}
}

void AudioSource::stop() {
	if(_state == playing || _state == pausing) {
		_state = stopping;
	}
}

void AudioSource::ignore() {
	if(_state == stopping) {
		_state = ignoring;
	}
}

SourceState AudioSource::getState() const {
	return _state;
}


}