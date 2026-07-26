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
template<class Char, glm::length_t N, class T, glm::qualifier Q>
struct std::formatter<glm::vec<N, T, Q>, Char> {
	constexpr formatter() {}

	constexpr formatter(const formatter&) {}

	constexpr formatter(formatter&&) {}

	constexpr formatter& operator=(const formatter&) { return *this; }

	constexpr formatter& operator=(formatter&&) { return *this; }

	template<class ParseContext>
	constexpr typename ParseContext::iterator parse(ParseContext& ctx) const {
		return std::range_formatter<T, Char>().parse(ctx);
	}

	/// @brief Formatting function
	/// @param vec - vector to format
	/// @param ctx - formatting context
	template<class FormatContext>
	constexpr typename FormatContext::iterator format(const glm::vec<N, T, Q>& vec, FormatContext& ctx) const {
		// format vec as range of values
		return std::range_formatter<T, Char>().format(std::span<const T, N>(glm::value_ptr(vec), N), ctx);
	}
};

static_assert(std::copy_constructible<std::range_formatter<float>>);

using F = std::formatter<glm::vec3>;

static_assert(std::copy_constructible<F>);
static_assert(std::is_move_constructible_v<F>);
static_assert(std::is_copy_assignable_v<F>);
static_assert(std::is_move_assignable_v<F>);

/// @brief Formatter for GLM matrices
/// @tparam C - matrix collumns
/// @tparam R - matrix rows
/// @tparam T - matrix type
/// @tparam Q - matrix qualifier
template<class Char, glm::length_t C, glm::length_t R, std::formattable<Char> T, glm::qualifier Q>
struct std::formatter<glm::mat<C, R, T, Q>, Char>: public std::range_formatter<glm::vec<R, T, Q>, Char> {
	using std::range_formatter<glm::vec<R, T, Q>, Char>::parse;

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
	using std::range_formatter<T, Char>::parse;

	/// @brief Formatting function
	/// @param qua - quaternion to format
	/// @param ctx - formatting context
	template<class FormatContext>
	constexpr typename FormatContext::iterator format(const glm::qua<T, Q>& qua, FormatContext& ctx) const;
};

#include "MathFormatter.hpp"
