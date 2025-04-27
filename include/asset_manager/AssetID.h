#pragma once

#include <string>
#include <filesystem>
#include <cstdint>
#include <functional>

namespace arch::assetManager {

class AssetID {
public:
	explicit AssetID(const std::filesystem::path& assetPath) noexcept;
	
	uint64_t Value() const noexcept;
	
	bool operator==(const AssetID& other) const noexcept;
	bool operator!=(const AssetID& other) const noexcept;


private:

	static uint64_t Hash(const std::filesystem::path& text);
	uint64_t _id = 0;

};

}

namespace std {

template<>
struct hash<arch::assetManager::AssetID> {
	size_t operator()(const arch::assetManager::AssetID& id) const noexcept { return static_cast<size_t>(id.Value()); }
};

}
