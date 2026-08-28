#include "TextBuilder.h"

namespace arch::text {

template<class C>
TextBuilder& TextBuilder::text(std::basic_string_view<C> str) {
	return _text(convertTo<char32_t>(str));
}

template<class C>
TextBuilder& TextBuilder::text(const std::basic_string<C>& str) {
	return _text(convertTo<char32_t>(str));
}

} // namespace arch::text
