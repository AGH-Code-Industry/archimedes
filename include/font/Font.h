#pragma once

#include <filesystem>
#include <ranges>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>

#include <utils/OptRef.h>
#include <utils/StringViewComparator.h>
#include <utils/StringViewHasher.h>

namespace arch::font {

/// @brief Regular font style constant
static constexpr std::string_view regular = "Regular";
/// @brief Bold font style constant
static constexpr std::string_view bold = "Bold";
/// @brief Italic font style constant
static constexpr std::string_view italic = "Italic";
/// @brief Bold Italic font style constant
/// @details Bold Italic is not a combination, but a separate style
static constexpr std::string_view boldItalic = "Bold Italic";

class MSDFAtlasGen;
class FontDB;
class Face;

/// @brief Represents font family
class Font {
	// unordered_map, but allows for using strings as keys
	using StylesSet = std::unordered_map<std::string_view, Face, utils::StringViewHasher, utils::StringViewComparator>;

public:

	/// @brief Checks if font has style
	/// @param style - style to check for
	bool hasStyle(std::string_view style) const noexcept;

	/// @brief Checks if font has Regular style
	bool hasRegular() const noexcept;
	/// @brief Checks if font has Italic style
	bool hasItalic() const noexcept;
	/// @brief Checks if font has Bold style
	bool hasBold() const noexcept;
	/// @brief Checks if font has Bold Italic style
	/// @details Bold Italic is not a combination, but a separate style
	bool hasBoldItalic() const noexcept;

	/// @brief Returns count of styles
	size_t styleCount() const noexcept;

	/// @brief Returns family name of font
	std::string_view name() const noexcept;
	/// @brief Returns optional with face of given fonst style
	/// @param style - name of style to get
	OptRef<Face> operator[](std::string_view style) noexcept;

	/// @brief Returns optional with face of given fonst style
	/// @param style - name of style to get, if empty returns Regular or first found style (optional, default value "")
	OptRef<Face> face(std::string_view style = "") noexcept;
	/// @brief Returns optional with face of Regular style
	OptRef<Face> regular() noexcept;
	/// @brief Returns optional with face of Bold style
	OptRef<Face> bold() noexcept;
	/// @brief Returns optional with face of Italic style
	OptRef<Face> italic() noexcept;
	/// @brief Returns optional with face of Bold Italic style
	/// @details Bold Italic is not a combination, but a separate style
	OptRef<Face> boldItalic() noexcept;

	/// @brief Returns view of avaliable styles
	auto styles() const noexcept -> decltype(std::views::keys(*std::declval<const StylesSet*>()));

private:

	friend class FontDB;

	Font() noexcept = default;

	std::string_view _familyName{};
	StylesSet _styles{};
};

} // namespace arch::font
