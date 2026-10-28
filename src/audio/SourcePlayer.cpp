#include <archimedes/audio/Calls.hpp>

#include <archimedes/audio/SourcePlayer.h>

namespace arch::audio {


void SourcePlayer::_loadClip(std::string_view path){
	// TODO:
	// send source id to the clip/sound bank (so it will store how many use it)
	// load data from clip to the source
	// (unloading will also use the id, the sound bank will manage the clips so none can be unloaded during playback)
	// (also the asset manager will store audio files instead of clips, probably)
}

void SourcePlayer::_updateParamsFromComponent(const AudioSourceComponent& source){
	alCall(alSourcef, _source, AL_PITCH, source.pitch);
	alCall(alSourcef, _source, AL_GAIN, source.gain);
	alCall(alSourcei, _source, AL_LOOPING, source.isLooped);
	alCall(alSourcef, _source, AL_MAX_DISTANCE, source.maxDistance);
	alCall(alSourcef, _source, AL_REFERENCE_DISTANCE, source.referenceDistance);
	alCall(alSourcef, _source, AL_ROLLOFF_FACTOR, source.rolloffFactor);
	alCall(alSourcef, _source, AL_CONE_INNER_ANGLE, source.coneInnerAngle);
	alCall(alSourcef, _source, AL_CONE_OUTER_ANGLE, source.coneOuterAngle);
	alCall(alSourcef, _source, AL_CONE_OUTER_GAIN, source.coneOuterGain);
	alCall(alSourcef, _source, AL_MIN_GAIN, source.minGain);
	alCall(alSourcef, _source, AL_MAX_GAIN, source.maxGain);
}

void SourcePlayer::clean() {
	_clipPath = "";
	AudioSourceComponent defaultSource{};
	_updateParamsFromComponent(defaultSource);
}

void SourcePlayer::update(const AudioSourceComponent& source) {
	if (source.path != _clipPath) {
		throw AudioException("Clip path shouldn't be changed during playback");
	}
	_updateParamsFromComponent(source);

}

void SourcePlayer::update(
	const AudioSourceComponent& source,
	const scene::components::TransformComponent& transform,
	const physics::RigidBodyComponent& moveable
) {
	update(source);
	alCall(alSource3f, _source, AL_POSITION, transform.position.x, transform.position.y, 0.0f);
	alCall(alSource3f, _source, AL_VELOCITY, moveable.linearVelocity.x, moveable.linearVelocity.y, 0.0f);
	if (source.isDirectional) {
		float3 direction = transform.rotation * float3(0.0f, 1.0f, 0.0f);
		alCall(alSource3f, _source, AL_DIRECTION, direction.x, direction.y, 0.0f);
	}
}

void SourcePlayer::stopPlaying() {
	alCall(alSourceStop, _source);
}

void SourcePlayer::pausePlaying() {
	alCall(alSourcePause, _source);
}

void SourcePlayer::initialize(SoundBank* soundBank) {
	_soundBank = soundBank;
	alCall(alGenSources, 1, &_source);
}

SourcePlayer::~SourcePlayer() {
	_soundBank = nullptr;
	alCall(alDeleteSources, 1, &_source);
}
} // namespace arch::audio
