#include <audio/AudioSourceComponent.h>

namespace arch::audio {
void AudioSourceComponent::play() {
	if (_state == pausing || _state == ignoring) {
		_state = playing;
	}
}

void AudioSourceComponent::pause() {
	if (_state == playing) {
		_state = pausing;
	}
}

void AudioSourceComponent::stop() {
	if (_state == playing || _state == pausing) {
		_state = stopping;
	}
}

void AudioSourceComponent::ignore() {
	if (_state == stopping) {
		_state = ignoring;
	}
}

SourceState AudioSourceComponent::getState() const {
	return _state;
}

} // namespace arch::audio
