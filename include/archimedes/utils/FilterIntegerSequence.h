#pragma once

#include <utility>

namespace arch::utils {

/// @brief Filters given integer_sequence
/// @details Use example: filterIntegerSequence<Pred(Ts)...>(std::make_index_sequence<sizeof...(Ts)>()) - returns
/// @details indexes of types in Ts... that satisfy Pred
/// @tparam Mask... - boolean mask: true retains integer, false does not
/// @param <unnamed> - integer_sequence to filter
template<bool... Mask, class T, T... Indexes>
consteval auto filterIntegerSequence(std::integer_sequence<T, Indexes...>) {
	constexpr auto N = sizeof...(Mask);
	static_assert(sizeof...(Indexes) == N, "filterIntegerSequence(): sizeof...(Mask) != sizeof...(Indexes)");

	// sum trues
	constexpr auto keepCount = ((size_t)0 + ... + Mask);

	// helper arrays for indexing
	constexpr auto mask = std::array{ Mask... };
	constexpr auto indexes = std::array{ Indexes... };

	constexpr auto result = [&] { // makes the result array
		std::array<T, keepCount> result;
		size_t i = 0;
		for (size_t j = 0; j != mask.size(); ++j) {
			if (mask[j]) {
				result[i++] = indexes[j];
			}
		}
		return result;
	}();

	// makes integer_sequence from result
	return [&]<size_t... ResultIndexes>(std::index_sequence<ResultIndexes...>) {
		return std::integer_sequence<T, result[ResultIndexes]...>();
	}(std::make_index_sequence<keepCount>());
}

} // namespace arch::utils

