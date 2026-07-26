#include <glm/gtc/type_ptr.hpp>

#include "MathFormatter.h"

template<class Char, glm::length_t N, std::formattable<Char> T, glm::qualifier Q>
template<class FormatContext>
constexpr typename FormatContext::iterator std::formatter<glm::vec<N, T, Q>, Char>::format(
	const glm::vec<N, T, Q>& vec,
	FormatContext& ctx
) const {
	// format vec as range of values
	return std::range_formatter<T, Char>::format(std::span<const T, N>(glm::value_ptr(vec), N), ctx);
}

template<class Char, glm::length_t C, glm::length_t R, std::formattable<Char> T, glm::qualifier Q>
template<class FormatContext>
constexpr typename FormatContext::iterator std::formatter<glm::mat<C, R, T, Q>, Char>::format(
	const glm::mat<C, R, T, Q>& mat,
	FormatContext& ctx
) const {
	// format mat as range of collumns
	auto colsSpan = std::span<const glm::vec<R, T, Q>, C>(std::addressof(mat[0]), C);
	return std::range_formatter<glm::vec<R, T, Q>, Char>::format(colsSpan, ctx);
}

template<class Char, std::formattable<Char> T, glm::qualifier Q>
template<class FormatContext>
constexpr typename FormatContext::iterator std::formatter<glm::qua<T, Q>, Char>::format(
	const glm::qua<T, Q>& qua,
	FormatContext& ctx
) const {
	// format qua as range of values
	return std::range_formatter<T, Char>::format(std::span<const T, 4>(glm::value_ptr(qua), 4), ctx);
}
