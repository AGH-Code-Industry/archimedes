#pragma once

#include <string>

#include <sndfile.h>

#include <archimedes/Math.h>


namespace arch::audio {

class AudioFile {

    SF_INFO _info;
    SNDFILE* _file;

    i32 _samples;

public:

    explicit AudioFile(const std::string& path);

    ~AudioFile();

    i32 getChannels() const;
    i32 getDataSize() const;
    i32 getSampleRate() const;

    void load(std::vector<i16>& buffer) const;

};
}