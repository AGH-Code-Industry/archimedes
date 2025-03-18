#pragma once

#include <codecvt>
#include <locale>
#include <string>
#include <string_view>

namespace arch::text {

/// @brief Converts input CharIn input to CharOut sting
/// @tparam CharOut - char type to convert to
/// @tparam CharIn - char type to convert from (deduced from input string)
/// @param string - string to convert
template<class CharOut, class CharIn>
std::basic_string<CharOut> convertTo(std::basic_string_view<CharIn> string) noexcept;

} // namespace arch::text

#include "ConvertTo.hpp"
