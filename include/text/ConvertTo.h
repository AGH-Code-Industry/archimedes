#pragma once

#include <codecvt>
#include <locale>
#include <string>
#include <string_view>

namespace arch::text {

template<class CharOut, class CharIn>
std::basic_string<CharOut> convertTo(std::basic_string_view<CharIn> string) noexcept;

} // namespace arch::text

#include "ConvertTo.hpp"
