#pragma once

#include "Get.hpp"
#include "Length.hpp"

// namespace arch {
// template<class... Ts>
// template<size_t Pos>
// struct TypeString<Ts...>::iterator {
//	using parent = TypeString<Ts...>;
//	static inline constexpr bool valid = (Pos < parent::length);
//	static inline constexpr size_t pos = (valid ? Pos : length);
//
//	using type = parent::get<Pos>;
//	using get = type;
//
//	template<size_t Val>
//	using add = iterator<Pos + Val>;
//	using next_t = std::conditional_t<valid, iterator<Pos + 1>, iterator>;
//	using next = next_t;
//	using prev = std::conditional_t<valid, std::conditional_t<(pos == 0), iterator, iterator<pos - 1>>, typename
// parent::iterator<length - 1>>;
//
//	template<IteratorOf<parent> It2>
//	static inline constexpr ptrdiff_t diff = (ptrdiff_t)pos - (ptrdiff_t)It2::pos;
// };
//
// template<class... Ts>
// struct TypeString<Ts...>::begin_s {
//	using type = iterator<0>;
// };
//
// template<class... Ts>
// struct TypeString<Ts...>::end_s {
//	using type = iterator<length>;
// };
// }
