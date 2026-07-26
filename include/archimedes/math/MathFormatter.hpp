#include <glm/gtc/type_ptr.hpp>

#include "MathFormatter.h"

template<size_t N, std::formattable<char> T, glm::qualifier Q>
template<class FormatContext>
constexpr FormatContext::iterator std::formatter<glm::vec<N, T, Q>>::format(
	const glm::vec<N, T, Q>& vec,
	FormatContext& ctx
) const {
	return std::range_formatter<T>::format(std::span<const T, N>(glm::value_ptr(vec), N), ctx);
}

template<size_t C, size_t R, std::formattable<char> T, glm::qualifier Q>
template<class FormatContext>
constexpr FormatContext::iterator std::formatter<glm::mat<C, R, T, Q>>::format(
	const glm::mat<C, R, T, Q>& mat,
	FormatContext& ctx
) const {
	auto colsSpan = std::span<const glm::vec<R, T, Q>, C>(&(mat[0]), C);
	return std::range_formatter<glm::vec<R, T, Q>>::format(colsSpan, ctx);
}

template<std::formattable<char> T, glm::qualifier Q>
template<class FormatContext>
constexpr FormatContext::iterator std::formatter<glm::qua<T, Q>>::format(
	const glm::qua<T, Q>& qua,
	FormatContext& ctx
) const {
	return std::range_formatter<T>::format(std::span<const T, 4>(glm::value_ptr(qua), 4), ctx);
}
