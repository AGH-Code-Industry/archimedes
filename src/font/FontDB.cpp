#include <fstream>
#include <vector>

#include <Logger.h>
#include <font/Font.h>
#include <font/FontDB.h>
#include <freetype2/ft2build.h>
#include <utils/Assert.h>
#include <windows.h>
#include FT_FREETYPE_H

namespace fs = std::filesystem;

namespace arch::font {

std::unique_ptr<FontDB> FontDB::_singleton;

std::vector<std::string_view> explodeStyle(std::string_view style) {
	std::vector<std::string_view> result;
	result.reserve(4);

	const char* substr = style.data();
	const char* end = substr + style.length();
	size_t i = 0;
	for (; substr + i < end; ++i) {
		if (substr[i] == ' ') {
			result.push_back(std::string_view(substr, i++));
			substr += i;
		}
	}
	result.push_back(std::string_view(substr, i));

	return result;
}

#if ARCHIMEDES_WINDOWS
void findAndAddFontsWindows(
	std::unordered_map<std::string, Font, utils::StringViewHasher, utils::StringViewComparator>& fonts,
	char* pimpl
) noexcept {
	FT_Library ft = (FT_Library)pimpl;
	for (const auto hRootKey : { HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER }) {
		constexpr LPCSTR subKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";

		HKEY hKey;
		if (RegOpenKeyEx(hRootKey, subKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
			arch::Logger::error("Cannot open registry key: {}", subKey);
			continue;
		}

		char valueName[256]{};
		DWORD valueNameSize = sizeof(valueName);

		unsigned char valueData[1'024]{};
		DWORD valueDataSize = sizeof(valueData);

		DWORD type;
		for (DWORD i = 0;; ++i, valueNameSize = sizeof(valueName), valueDataSize = sizeof(valueData)) {
			auto result = RegEnumValue(hKey, i, valueName, &valueNameSize, NULL, &type, valueData, &valueDataSize);
			if (result == ERROR_NO_MORE_ITEMS) {
				break;
			} else if (result == ERROR_SUCCESS && type == REG_SZ) {
				// we have path to font file

				auto path = std::string((char*)valueData);
				if (!fs::path(path).has_parent_path()) {
					path = "C:/Windows/Fonts/" + path;
				}

				FT_Face face{};
				FT_Long faceCount = 0;
				FT_Long i = 0;

				do {
					face = {};
					if (FT_New_Face(ft, path.c_str(), i, &face) == 0) {
						if (faceCount == 0) {
							faceCount = face->num_faces;
						}

						auto foundFont = fonts.find(face->family_name);

						// set familyName
						if (foundFont == fonts.end()) {
							auto&& [it, ignore] = fonts.insert({ face->family_name, {} });

							it->second._familyName = it->first;
							foundFont = it;
						} else if (foundFont->second.familyName().empty()) {
							foundFont->second._familyName = foundFont->first;
						}

						auto&& font = foundFont->second;

						font._styles.insert({
							face->style_name,
							{ path, (char*)face }
						  });
					}
				} while (++i < faceCount);
			}
		}
	}
}
#elif ARCHIMEDES_LINUX
void findAndAddFontsLinux(
	std::unordered_map<std::string, Font, StringViewHasher, StringViewComparator>& fonts,
	char* pimpl
) noexcept {}
#endif

void FontDB::_findAndAddFonts() noexcept {
#if ARCHIMEDES_WINDOWS
	findAndAddFontsWindows(_fonts, _pimpl);
#endif
}

FontDB::FontDB() noexcept {
	FT_Init_FreeType(reinterpret_cast<FT_Library*>(&_pimpl));
	_findAndAddFonts();
}

FontDB::~FontDB() noexcept {
	_fonts.clear();
	FT_Done_FreeType(reinterpret_cast<FT_Library>(_pimpl));
}

FontDB& FontDB::get() noexcept {
	if (!_singleton) {
		_singleton = std::unique_ptr<FontDB>(new FontDB());
	}
	return *_singleton;
}

bool FontDB::exists(std::string_view familyName) const noexcept {
	return _fonts.contains(familyName);
}

OptRef<Font> FontDB::get(std::string_view familyName) noexcept {
	auto found = _fonts.find(familyName);
	if (found != _fonts.end()) {
		return found->second;
	}
	return std::nullopt;
}

FontDB::MapType::const_iterator FontDB::begin() const noexcept {
	return _fonts.cbegin();
}

FontDB::MapType::const_iterator FontDB::end() const noexcept {
	return _fonts.cend();
}

} // namespace arch::font
