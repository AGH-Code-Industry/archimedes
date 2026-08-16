#include "SourceAwareFormatString.h"

namespace arch::utils {

template<class SourceLocation, class Char, class... Args>
template<class T>
requires std::convertible_to<const T&, std::basic_string_view<Char>>
consteval BasicSourceAwareFormatString<SourceLocation, Char, Args...>::BasicSourceAwareFormatString(
	const T& str,
	const SourceLocation location
):
	fmt(str),
	loc{ location } {}

} // namespace arch::utils
