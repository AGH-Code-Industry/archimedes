#include <audio/Clip.h>
#include <audio/AudioException.h>
#include <sndfile.h>

namespace arch::audio{

	ALenum Clip::_chooseFormat(const ALushort& channelsNumber) const {
		if (channelsNumber == 1){
			return AL_FORMAT_MONO16;
		}
		if(channelsNumber == 2){
			return AL_FORMAT_STEREO16;
		}
		throw AudioException(_filePath + " - wrong format");
	}


	void Clip::load(){
		SF_INFO soundInfo;
		SNDFILE *soundFile;
		ALint samples;
		soundInfo.format = 0;
		soundFile = sf_open(_filePath.c_str(), SFM_READ, &soundInfo);
		samples = soundInfo.channels * soundInfo.frames;
		_data.reserve(samples);
		sf_read_short(soundFile, &_data[0], samples);
		sf_close(soundFile);
		_format = _chooseFormat(soundInfo.channels);
		_dataSize = samples * sizeof(short);
		_sampleRate = soundInfo.samplerate;
	}

	void Clip::unload(){
		_data.clear();
	}

	Clip::~Clip(){
		unload();
	}

	const ALshort* Clip::getData() const {
		return _data.data();
	}

	ALenum Clip::getFormat() const {
		return _format;
	}

	ALint Clip::getSize() const {
		return _dataSize;
	}

	ALint Clip::getSampleRate() const {
		return _sampleRate;
	}
}