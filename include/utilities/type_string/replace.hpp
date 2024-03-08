#pragma once


namespace arch {
template<class... Ts>
template<size_t Pos, size_t Count, class... Ts2>
struct type_string<Ts...>::replace_s {
	using type = erase<Pos, Count>::template insert<Pos, Ts2...>;
};

template<class... Ts>
template<IteratorOf<type_string<Ts...>> First, IteratorOf<type_string<Ts...>> Last, class... Ts2>
struct type_string<Ts...>::replace_i_s {
	using type = erase_i<First, Last>::template insert<First::pos, Ts2...>;
};

template<class... Ts>
template<size_t Pos, size_t Count, class Str>
struct type_string<Ts...>::replace_from_s {
	using type = erase<Pos, Count>::template insert_from<Pos, Str>;
};

template<class... Ts>
template<IteratorOf<type_string<Ts...>> First, IteratorOf<type_string<Ts...>> Last, class Str>
struct type_string<Ts...>::replace_from_i_s {
	using type = erase_i<First, Last>::template insert_from<First::pos, Str>;
};

template<class... Ts>
template<size_t Pos, size_t Count, class... Strs>
struct type_string<Ts...>::replace_from_cat_s {
	using type = erase<Pos, Count>::template insert_from_cat<Pos, Strs...>;
};

template<class... Ts>
template<IteratorOf<type_string<Ts...>> First, IteratorOf<type_string<Ts...>> Last, class... Strs>
struct type_string<Ts...>::replace_from_cat_i_s {
	using type = erase_i<First, Last>::template insert_from_cat<First::pos, Strs...>;
};

//type_string<int, float, char>::bind_iterator<ts_begin>;

//type_string<int, void, void, size_t, void>::replace_i<type_string<int, void, void, size_t, void>::begin::add<1>, type_string<int, void, void, size_t, void>::begin::add<3>, float, char>
}