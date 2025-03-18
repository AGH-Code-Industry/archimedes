#pragma once

#include <utility>

namespace arch::utils {

template<class T1, class T2 = T1>
struct PairHasher {
	using argument_type = std::pair<T1, T2>;
	using result_type = std::size_t;

	result_type operator()(const argument_type& pair) const noexcept;
};

} // namespace arch::utils

#include "PairHasher.hpp"
