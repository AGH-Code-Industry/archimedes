#pragma once

#include <concepts>
#include <format>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Math.h"

/// @brief Formatter for GLM vectors
/// @tparam N - vector size
/// @tparam T - vector type
/// @tparam Q - vector qualifier
template<class Char, glm::length_t N, std::formattable<Char> T, glm::qualifier Q>
struct std::formatter<glm::vec<N, T, Q>, Char>: public std::range_formatter<T, Char> {
	/// @brief Formatting function
	/// @param vec - vector to format
	/// @param ctx - formatting context
	template<class FormatContext>
	constexpr typename FormatContext::iterator format(const glm::vec<N, T, Q>& vec, FormatContext& ctx) const;
};

/// @brief Formatter for GLM matrices
/// @tparam C - matrix collumns
/// @tparam R - matrix rows
/// @tparam T - matrix type
/// @tparam Q - matrix qualifier
template<class Char, glm::length_t C, glm::length_t R, std::formattable<Char> T, glm::qualifier Q>
struct std::formatter<glm::mat<C, R, T, Q>, Char>: public std::range_formatter<glm::vec<R, T, Q>, Char> {
	/// @brief Formatting function
	/// @param mat - matrix to format
	/// @param ctx - formatting context
	template<class FormatContext>
	constexpr typename FormatContext::iterator format(const glm::mat<C, R, T, Q>& mat, FormatContext& ctx) const;
};

/// @brief Formatter for GLM quaternions
/// @tparam T - quaternion type
/// @tparam Q - quaternion qualifier
template<class Char, std::formattable<Char> T, glm::qualifier Q>
struct std::formatter<glm::qua<T, Q>, Char>: public std::range_formatter<T, Char> {
	/// @brief Formatting function
	/// @param qua - quaternion to format
	/// @param ctx - formatting context
	template<class FormatContext>
	constexpr typename FormatContext::iterator format(const glm::qua<T, Q>& qua, FormatContext& ctx) const;
};

#include "MathFormatter.hpp"
