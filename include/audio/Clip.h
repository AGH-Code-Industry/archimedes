#pragma once

#include <AL/alc.h>
#include <AL/al.h>
#include <string>
#include <vector>


namespace arch::audio{

	class Clip{
		ALint _sampleRate{}, _dataSize{};
		ALushort _channelsNumber{};
		std::vector<ALshort> _soundData;
		std::string _filePath;
		const std::size_t _buffersItems = 16384;

		public:

			Clip(const std::string& path) : _filePath(path) {}
			void load();
			void unload();
			~Clip();
			const ALshort* getData() const;
			bool fillBuffer(std::vector<ALshort>& buffer, std::size_t& cursor, bool isLooped) const;
			std::size_t getBufferSize() const;
			ALenum getFormat() const;
			ALint getSize() const;
			ALint getSampleRate() const;
	};
}