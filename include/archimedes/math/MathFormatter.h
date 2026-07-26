#pragma once

#include <concepts>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Math.h"

template<size_t N, std::formattable<char> T, glm::qualifier Q>
struct std::formatter<glm::vec<N, T, Q>>: std::range_formatter<T> {
	template<class FormatContext>
	constexpr FormatContext::iterator format(const glm::vec<N, T, Q>& vec, FormatContext& ctx) const;
};

template<size_t C, size_t R, std::formattable<char> T, glm::qualifier Q>
struct std::formatter<glm::mat<C, R, T, Q>>: public std::range_formatter<glm::vec<R, T, Q>> {
	template<class FormatContext>
	constexpr FormatContext::iterator format(const glm::mat<C, R, T, Q>& mat, FormatContext& ctx) const;
};

template<std::formattable<char> T, glm::qualifier Q>
struct std::formatter<glm::qua<T, Q>>: public std::range_formatter<T> {
	template<class FormatContext>
	constexpr FormatContext::iterator format(const glm::qua<T, Q>& qua, FormatContext& ctx) const;
};

#include "MathFormatter.hpp"
