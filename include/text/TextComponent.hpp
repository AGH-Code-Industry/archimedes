#include <format>

#include "ConvertTo.h"
#include "TextComponent.h"
#include <Font.h>

namespace arch::text {

template<class Char>
TextComponent& TextComponent::operator=(std::basic_string_view<Char> text) {
	return setText(text);
}

template<class Char>
TextComponent& TextComponent::setText(std::basic_string_view<Char> text) {
	_text = convertTo<char32_t>(text);
	return *this;
}

} // namespace arch::text
