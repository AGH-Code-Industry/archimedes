#include <algorithm>
#include <cctype>
#include <fstream>
#include <unordered_set>
#include <vector>

#include <archimedes/Logger.h>
#include <archimedes/font/Face.h>
#include <archimedes/font/Font.h>
#include <archimedes/font/FontDB.h>
#include <archimedes/utils/Assert.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#if ARCHIMEDES_WINDOWS
#include <windows.h>
#elif ARCHIMEDES_LINUX
#endif

namespace fs = std::filesystem;

namespace arch::font {

std::unique_ptr<FontDB> FontDB::_singleton;

void FontDB::_addFont(std::string path) noexcept {
	if constexpr (ARCHIMEDES_WINDOWS) {
		// some fonts dont have full path in registry
		if (!fs::path(path).has_parent_path()) {
			path = "C:/Windows/Fonts/" + path;
		}
	}

	FT_Face face{};
	FT_Long faceCount = 0;
	FT_Long i = 0;

	auto ft = reinterpret_cast<FT_Library>(_pimpl);

	// font file can have multiple styles inside
	do {
		face = {};
		if (FT_New_Face(ft, path.c_str(), i, &face) == 0) {
			if (faceCount == 0) {
				faceCount = face->num_faces;
			}

			auto foundFont = _fonts.find(face->family_name);

			// set name
			if (foundFont == _fonts.end()) {
				auto&& [it, ignore] = _fonts.insert({ face->family_name, {} });

				it->second._familyName = it->first;
				foundFont = it;
			} else if (foundFont->second.name().empty()) {
				foundFont->second._familyName = foundFont->first;
			}

			auto&& font = foundFont->second;

			// add style
			auto&& [it, ignored] = font._styles.insert({ face->style_name, {} });
			auto&& fontFace = it->second;
			fontFace._familyName = font._familyName;
			fontFace._fontPath = path;
			fontFace._pimpl = (char*)face;
		}
	} while (++i < faceCount);
}

#if ARCHIMEDES_WINDOWS
void FontDB::_findAndAddFontsWindows() noexcept {
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
				_addFont((char*)valueData);
			}
		}
	}
}
#elif ARCHIMEDES_LINUX
void FontDB::_findAndAddFontsLinux() noexcept {
	const auto fontDirs = std::initializer_list<fs::path>{ "/usr/share/fonts",
														   "/usr/local/share/fonts",
														   "/opt/share/fonts",
														   "~/.fonts",
														   "~/.local/share/fonts",
														   "~/.config/fontconfig/fonts",
														   "~/.wine/drive_c/windows/Fonts" };

	const std::unordered_set<std::string_view> extensions{ ".ttf", ".otf", ".woff", ".woff2", ".pfa", ".pfb", ".cff",
														   ".pcf", ".bdf", ".bdf",	".pfr",	  ".fnt", ".fon" };

	std::error_code ignored;
	for (auto&& dir : fontDirs) {
		if (fs::is_directory(dir, ignored)) {
			for (auto&& entry :
				 fs::recursive_directory_iterator(dir, fs::directory_options::skip_permission_denied, ignored)) {
				std::string path = entry.path().string();
				std::string extension = entry.path().extension().string();
				// linux and its quirks strike again
				std::ranges::for_each(extension, [](char& c) { c = std::tolower(c); });
				if (extensions.contains(extension)) {
					_addFont(std::move(path));
				}
			}
		}
	}
}
#endif

void FontDB::_findAndAddFonts() noexcept {
#if ARCHIMEDES_WINDOWS
	_findAndAddFontsWindows();
#elif ARCHIMEDES_LINUX
	_findAndAddFontsLinux();
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

bool FontDB::exists(std::string_view name) const noexcept {
	return _fonts.contains(name);
}

OptRef<Font> FontDB::get(std::string_view name) noexcept {
	auto found = _fonts.find(name);
	if (found != _fonts.end()) {
		return found->second;
	}
	return std::nullopt;
}

OptRef<Font> FontDB::operator[](std::string_view name) noexcept {
	return get(name);
}

FontDB::MapType::const_iterator FontDB::begin() const noexcept {
	return _fonts.cbegin();
}

FontDB::MapType::const_iterator FontDB::end() const noexcept {
	return _fonts.cend();
}

} // namespace arch::font
