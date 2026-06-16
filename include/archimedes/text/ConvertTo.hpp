#include "ConvertTo.h"

template<class CharOut, class CharIn>
std::basic_string<CharOut> arch::text::convertTo(std::basic_string_view<CharIn> string) noexcept {
	if constexpr (std::same_as<CharIn, CharOut>) {
		return std::basic_string<CharOut>(string);
	} else if constexpr (std::same_as<CharIn, char>) {
		std::wstring_convert<std::codecvt_utf8<CharOut>, CharOut> outConverter;
		return outConverter.from_bytes(string.data(), string.data() + string.length());
	} else if constexpr (std::same_as<CharOut, char>) {
		std::wstring_convert<std::codecvt_utf8<CharIn>, CharIn> inConverter;
		return inConverter.to_bytes(string.data(), string.data() + string.length());
	} else {
		std::wstring_convert<std::codecvt_utf8<CharIn>, CharIn> inConverter;
		std::wstring_convert<std::codecvt_utf8<CharOut>, CharOut> outConverter;

		return outConverter.from_bytes(inConverter.to_bytes(string.data(), string.data() + string.length()));
	}
}
