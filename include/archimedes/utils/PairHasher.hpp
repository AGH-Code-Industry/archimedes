#include "PairHasher.h"

template<class T1, class T2>
arch::utils::PairHasher<T1, T2>::result_type arch::utils::PairHasher<T1, T2>::operator()(const argument_type& pair
) const noexcept {
	return std::hash<T1>{}(pair.first) ^ (std::hash<T2>{}(pair.second) << 1);
}
