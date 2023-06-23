#include "resource/assimp_init.h"

#include <spdlog/spdlog.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/Logger.hpp>

using namespace arch;

void SpdlogStream::write(const char *msg) {
    spdlog::debug("{}", msg);
}

void AssimpInitializer::init() {
    init_logger();
}

void AssimpInitializer::init_logger() {
    const unsigned int severity = 
        Assimp::Logger::Debugging | Assimp::Logger::Info |
        Assimp::Logger::Err | Assimp::Logger::Warn;
    Assimp::DefaultLogger::get()->attachStream(new SpdlogStream, severity);
}