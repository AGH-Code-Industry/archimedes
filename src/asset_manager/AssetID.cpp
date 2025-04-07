#include <asset_manager/AssetID.h>

namespace arch::assetManager {

	AssetID::AssetID(std::string assetPath) noexcept: _id(Hash(assetPath)) {}

	uint64_t AssetID::Value() const noexcept {
		return _id;
	}

	bool AssetID::operator==(const AssetID& other) const noexcept {
		return _id == other._id;
	}

	bool AssetID::operator!=(const AssetID& other) const noexcept {
		return !(_id == other._id);
	}

	uint64_t AssetID::Hash(const std::string& text) {
		static const uint64_t fnvOffsetBasis{ 14'695'981'039'346'656'037ULL };
		static const uint64_t fnvPrime{ 1'099'511'628'211ULL };
		
		uint64_t hash{ fnvOffsetBasis };
		for (char c : text) {
			hash ^= static_cast<unsigned char>(c);
			hash *= fnvPrime;
		}

		return hash;
	}

} // namespace arch::assetManager
