#pragma once

#include <concepts>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Math.h"

/// @brief Formatter for GLM vectors
/// @tparam N - vector size
/// @tparam T - vector type
/// @tparam Q - vector qualifier
template<size_t N, std::formattable<char> T, glm::qualifier Q>
struct std::formatter<glm::vec<N, T, Q>>: std::range_formatter<T> {
	/// @brief Formatting function
	/// @param vec - vector to format
	/// @param ctx - formatting context
	template<class FormatContext>
	constexpr FormatContext::iterator format(const glm::vec<N, T, Q>& vec, FormatContext& ctx) const;
};

/// @brief Formatter for GLM matrices
/// @tparam C - matrix collumns
/// @tparam R - matrix rows
/// @tparam T - matrix type
/// @tparam Q - matrix qualifier
template<size_t C, size_t R, std::formattable<char> T, glm::qualifier Q>
struct std::formatter<glm::mat<C, R, T, Q>>: public std::range_formatter<glm::vec<R, T, Q>> {
	/// @brief Formatting function
	/// @param mat - matrix to format
	/// @param ctx - formatting context
	template<class FormatContext>
	constexpr FormatContext::iterator format(const glm::mat<C, R, T, Q>& mat, FormatContext& ctx) const;
};

/// @brief Formatter for GLM quaternions
/// @tparam T - quaternion type
/// @tparam Q - quaternion qualifier
template<std::formattable<char> T, glm::qualifier Q>
struct std::formatter<glm::qua<T, Q>>: public std::range_formatter<T> {
	/// @brief Formatting function
	/// @param qua - quaternion to format
	/// @param ctx - formatting context
	template<class FormatContext>
	constexpr FormatContext::iterator format(const glm::qua<T, Q>& qua, FormatContext& ctx) const;
};

#include "MathFormatter.hpp"
