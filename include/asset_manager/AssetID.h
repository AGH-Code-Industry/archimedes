#pragma once

#include <string>

namespace arch::assetManager {
class AssetID {
public:
	explicit AssetID(std::string assetPath) noexcept;
	
	uint64_t Value() const noexcept;
	
	bool operator==(const AssetID& other) const noexcept;
	bool operator!=(const AssetID& other) const noexcept;


private:

	static uint64_t Hash(const std::string& text);
	uint64_t _id;

};
}
