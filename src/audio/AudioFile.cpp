#include <archimedes/audio/AudioFile.h>
#include <archimedes/audio/AudioException.h>

namespace arch::audio {
    AudioFile::AudioFile(const std::string& path){
        _info.format = 0;
        _file = sf_open(path.c_str(), SFM_READ, &_info);
        if (_file == nullptr) {
            std::string errorMessage = std::string(sf_strerror(_file));
            throw AudioException(path + " - " + errorMessage);
        }
        _samples = _info.channels * _info.frames;
    }

    AudioFile::~AudioFile(){
        sf_close(_file);
    }

    i32 AudioFile::getChannels() const{
        return _info.channels;
    }

    i32 AudioFile::getSampleRate() const{
        return _info.samplerate;
    }

    void AudioFile::load(std::vector<i16>& buffer) const {
        buffer.resize(_samples);
        sf_read_short(_file, buffer.data(), _samples);
    }

}