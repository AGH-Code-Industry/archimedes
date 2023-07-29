#pragma once

#include <filesystem>

template<typename Resource>
class ResourceLoader {
public:
    virtual Resource read_file(const std::filesystem::path &filepath);
};
