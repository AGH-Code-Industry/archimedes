#pragma once

#include "TypeList.hpp"

#include "TypeListFwd.h"

#define TL_ERROR(msg) static_assert(details::alwaysFalse<SIZE>, msg)

namespace arch::utils {

template<class... Types, class... Types2>
static consteval auto operator==(TypeList<Types...>, TypeList<Types2...>) {
	return std::same_as<TypeList<Types...>, TypeList<Types2...>>;
}

template<class... Types, class... Types2>
static consteval auto operator!=(TypeList<Types...>, TypeList<Types2...>) {
	return !std::same_as<TypeList<Types...>, TypeList<Types2...>>;
}

template<class... Types>
struct TypeList: details::SingleTypeAlias<sizeof...(Types) == 1, Types...> {
private:
	using This = TypeList<Types...>;
	static inline constexpr size_t SIZE = sizeof...(Types);

	template<size_t I>
	using Get = std::conditional_t<SIZE != 0, typename details::TLGet<I, Types...>::type, void>;

public:

	static inline constexpr size_t npos = (size_t)-1;

	static consteval auto emtpy() { return SIZE == 0; }

	static consteval auto size() { return SIZE; }

	static consteval auto length() { return SIZE; }

	template<size_t I>
	static consteval auto get() {
		if constexpr (SIZE == 0 || I >= SIZE) {
			TL_ERROR("TypeList::get: index out of bounds");
			return typelist<>;
		} else {
			return typelist<Get<I>>;
		}
	}

	static consteval auto front() {
		if constexpr (SIZE == 0) {
			TL_ERROR("TypeList::front: empty typelist");
			return typelist<>;
		} else {
			return get<0>();
		}
	}

	static consteval auto back() {
		if constexpr (SIZE < 2) {
			static_assert(SIZE != 0, "TypeList::back: empty typelist");
			return This();
		} else {
			return get<SIZE - 1>();
		}
	}

	template<size_t N = 1>
	static consteval auto popFront() {
		if constexpr (SIZE == 0) {
			TL_ERROR("TypeList::popFront: empty typelist");
			return typelist<>;
		} else if constexpr (SIZE < N) {
			TL_ERROR("TypeList::popFront: N bigger that typelist.size()");
			return typelist<>;
		} else {
			return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typelist<Get<Indexes + N>...>;
			}(std::make_index_sequence<SIZE - N>());
		}
	}

	template<size_t N = 1>
	static consteval auto popBack() {
		if constexpr (SIZE == 0) {
			TL_ERROR("TypeList::popBack: empty typelist");
			return typelist<>;
		} else if constexpr (SIZE < N) {
			TL_ERROR("TypeList::popBack: N bigger that typelist.size()");
			return typelist<>;
		} else {
			return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typelist<Get<Indexes>...>;
			}(std::make_index_sequence<SIZE - N>());
		}
	}

	template<size_t Begin = 0, size_t Count = npos>
	static consteval auto sublist() {
		if constexpr (Begin > SIZE) {
			TL_ERROR("TypeList::sublist: Begin bigger that typelist.size()");
			return typelist<>;
		} else {
			constexpr auto sublistSize = std::min(Count, SIZE - Begin);

			return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typelist<Get<Indexes + Begin>...>;
			}(std::make_index_sequence<sublistSize>());
		}
	}

	template<size_t Pos, class... Types2>
	static consteval auto insert(TypeList<Types2...> other) {
		if constexpr (Pos > SIZE) {
			TL_ERROR("Typelist::insert: Pos out of range");
			return typelist<>;
		} else {
			constexpr auto rightSize = SIZE - Pos;

			return []<size_t... Head, size_t... Tail>(std::index_sequence<Head...>, std::index_sequence<Tail...>) {
				return typelist<Get<Head>..., Types2..., Get<Tail + Pos>...>;
			}(std::make_index_sequence<Pos>(), std::make_index_sequence<rightSize>());
		}
	}

	template<size_t Begin = 0, size_t Count = npos>
	static consteval auto erase() {
		if constexpr (Begin > SIZE) {
			TL_ERROR("Typelist::erase: Begin out of range");
			return typelist<>;
		} else {
			constexpr auto toErase = std::min(Count, SIZE - Begin);
			constexpr auto rightBegin = Begin + toErase;
			constexpr auto rightSize = SIZE - rightBegin;

			return []<size_t... Head, size_t... Tail>(std::index_sequence<Head...>, std::index_sequence<Tail...>) {
				return typelist<Get<Head>..., Get<Tail + rightBegin>...>;
			}(std::make_index_sequence<Begin>(), std::make_index_sequence<rightSize>());
		}
	}

	template<class... Types2>
	static consteval auto append(TypeList<Types2...>) {
		return typelist<Types..., Types2...>;
	}

	template<class... Types2>
	consteval auto operator+(TypeList<Types2...> other) const {
		return append(other);
	}

	template<class... Types2>
	static consteval auto prepend(TypeList<Types2...>) {
		return typelist<Types2..., Types...>;
	}

	template<class... Others>
	static consteval auto cat(Others...) {
		return typename details::TLCat<This, Others...>::type();
	}

	template<size_t Begin = 0, size_t Count = npos, class... Types2>
	static consteval auto replace(TypeList<Types2...> other) {
		if constexpr (Begin > SIZE) {
			TL_ERROR("Typelist::replace: Begin out of range");
			return typelist<>;
		} else {
			constexpr auto toErase = std::min(Count, SIZE - Begin);
			constexpr auto rightBegin = Begin + toErase;
			constexpr auto rightSize = SIZE - rightBegin;

			return []<size_t... Head, size_t... Tail>(std::index_sequence<Head...>, std::index_sequence<Tail...>) {
				return typelist<Get<Head>..., Types2..., Get<Tail + rightBegin>...>;
			}(std::make_index_sequence<Begin>(), std::make_index_sequence<rightSize>());
		}
	}

	template<size_t Begin = 0, size_t Count = -1, class... Types2>
	static consteval size_t find(TypeList<Types2...>) {
		constexpr auto other = typelist<Types2...>;

		if constexpr (other.size() == 0) {
			return (Begin <= SIZE) ? Begin : 0;
		} else if constexpr (Begin >= SIZE || other.size() > SIZE) {
			return npos;
		} else {
			constexpr auto searchEnd = std::min(Count >= SIZE - Begin ? SIZE : Begin + Count, SIZE - other.size() + 1);

			if constexpr (Begin >= searchEnd) {
				return npos;
			} else {
				return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
					size_t result = npos;
					(... ||
					 ((result == npos && sublist<Begin + Indexes, other.size()>() == other) ?
						  (result = Begin + Indexes, true) :
						  false));
					return result;
				}(std::make_index_sequence<searchEnd - Begin>());
			}
		}
	}

	template<size_t Begin = npos, size_t Count = -1, class... Types2>
	static consteval size_t rfind(TypeList<Types2...>) {
		constexpr auto other = typelist<Types2...>;

		if constexpr (other.size() == 0) {
			return (Begin <= SIZE) ? Begin : SIZE;
		} else if constexpr (SIZE == 0 || other.size() > SIZE) {
			return npos;
		} else {
			constexpr auto lastStart = SIZE - other.size();
			constexpr auto searchBegin = std::min(Begin == npos ? lastStart : Begin, lastStart);
			constexpr auto searchEnd = (Count == npos) ? 0 : (searchBegin >= Count ? searchBegin - Count : 0);

			if constexpr (searchBegin < searchEnd) {
				return npos;
			} else {
				return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
					size_t result = npos;
					(... ||
					 ((result == npos && sublist<searchBegin - Indexes, other.size()>() == other) ?
						  (result = searchBegin - Indexes, true) :
						  false));
					return result;
				}(std::make_index_sequence<searchBegin - searchEnd + 1>());
			}
		}
	}

	template<class... Types2>
	static consteval auto contains(TypeList<Types2...> other) {
		return find(other) != npos;
	}

	template<class... Types2>
	static consteval auto containsAll(TypeList<Types2...>) {
		return (... && contains(TypeList<Types2>()));
	}

	template<class... Types2>
	static consteval auto startsWith(TypeList<Types2...> other) {
		return find(other) == 0;
	}

	template<class... Types2>
	static consteval auto endsWith(TypeList<Types2...> other) {
		return other.size() <= size() && rfind(other) == size() - other.size();
	}

	template<auto Pred, size_t Begin = 0, size_t Count = npos>
	static consteval auto eraseIf() {
		if constexpr (SIZE != 0 && Begin >= SIZE) {
			TL_ERROR("TypeList::eraseIf: Begin out of range");
		} else if constexpr (SIZE == 0 || Count == 0) {
			return This();
		} else {
			constexpr auto searchEnd = Count >= SIZE - Begin ? SIZE : std::min(Begin + Count, SIZE);

			return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typename details::TLCat<typename details::SingleFilter<
					(Indexes < Begin || Indexes >= searchEnd || !Pred(typelist<Get<Indexes>>)),
					Get<Indexes>>::type...>::type();
			}(std::make_index_sequence<SIZE>());
		}
	}

	template<template<class T> class TypeTrait, size_t Begin = 0, size_t Count = npos>
	static consteval auto eraseIf() {
		if constexpr (SIZE != 0 && Begin >= SIZE) {
			TL_ERROR("TypeList::eraseIf: Begin out of range");
		} else if constexpr (SIZE == 0 || Count == 0) {
			return This();
		} else {
			constexpr auto searchEnd = Count >= SIZE - Begin ? SIZE : std::min(Begin + Count, SIZE);

			return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typename details::TLCat<typename details::SingleFilter<
					(Indexes < Begin || Indexes >= searchEnd || !TypeTrait<Get<Indexes>>::value),
					Get<Indexes>>::type...>::type();
			}(std::make_index_sequence<SIZE>());
		}
	}

	template<size_t Begin = 0, size_t Count = npos>
	static consteval auto distinct() {
		if constexpr (SIZE != 0 && Begin >= SIZE) {
			TL_ERROR("TypeList::distinct: Begin out of range");
		} else if constexpr (SIZE == 0 || Count == 0) {
			return This{};
		} else {
			constexpr auto searchEnd = (Count >= SIZE - Begin) ? SIZE : std::min(Begin + Count, SIZE);

			return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typename details::TLCat<typename details::SingleFilter<
					(Indexes < Begin || Indexes >= searchEnd ||
					 find<Begin, searchEnd - Begin>(typelist<Get<Indexes>>) == Indexes),
					Get<Indexes>>::type...>::type();
			}(std::make_index_sequence<SIZE>());
		}
	}

	template<auto Pred, size_t Begin = 0, size_t Count = npos>
	static consteval auto filter() {
		static_assert(SIZE == 0 || Begin <= SIZE, "TypeList::filter: Begin out of range");
		return eraseIf<[](auto tl) { return !Pred(tl); }, Begin, Count>();
	}

	template<template<class T> class TypeTrait, size_t Begin = 0, size_t Count = npos>
	static consteval auto filter() {
		static_assert(SIZE == 0 || Begin <= SIZE, "TypeList::filter: Begin out of range");
		return eraseIf<details::NotTrait<TypeTrait>::template type, Begin, Count>();
	}

	template<size_t Begin = 0, size_t Count = npos>
	static consteval auto reverse() {
		if constexpr (Begin >= SIZE) {
			TL_ERROR("TypeList::reverse: Begin out of range");
			return typelist<>;
		} else {
			constexpr auto reverseEnd = (Count >= SIZE - Begin) ? SIZE : std::min(Begin + Count, SIZE);
			constexpr auto getIndex = [](size_t i) {
				if (Begin <= i && i < reverseEnd) {
					return (reverseEnd - 1) - (i - Begin);
				}
				return i;
			};

			return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typelist<Get<getIndex(Indexes)>...>;
			}(std::make_index_sequence<SIZE>());
		}
	}

	template<auto Fn, size_t Begin = 0, size_t Count = npos>
	static consteval auto transform() {
		if constexpr (Begin >= SIZE) {
			TL_ERROR("TypeList::transform: Begin out of range");
			return typelist<>;
		} else {
			constexpr auto transformEnd = (Count >= SIZE - Begin) ? SIZE : std::min(Begin + Count, SIZE);

			return []<size_t... Indexes, size_t... Head, size_t... Tail>(
					   std::index_sequence<Indexes...>,
					   std::index_sequence<Head...>,
					   std::index_sequence<Tail...>
				   ) {
				return typelist<Get<Head>...>.cat(
					Fn(typelist<Get<Indexes + Begin>>)...,
					typelist<Get<Tail + transformEnd>...>
				);
			}(std::make_index_sequence<transformEnd - Begin>(),
				   std::make_index_sequence<Begin>(),
				   std::make_index_sequence<SIZE - transformEnd>());
		}
	}

	template<template<class T> class TypeTrait, size_t Begin = 0, size_t Count = npos>
	static consteval auto transform() {
		if constexpr (Begin >= SIZE) {
			TL_ERROR("TypeList::transform: Begin out of range");
			return typelist<>;
		} else {
			constexpr auto transformEnd = (Count >= SIZE - Begin) ? SIZE : std::min(Begin + Count, SIZE);

			return []<size_t... Indexes, size_t... Head, size_t... Tail>(
					   std::index_sequence<Indexes...>,
					   std::index_sequence<Head...>,
					   std::index_sequence<Tail...>
				   ) {
				return typelist<Get<Head>...>.cat(
					typelist<typename TypeTrait<Get<Indexes + Begin>>::type>...,
					typelist<Get<Tail + transformEnd>...>
				);
			}(std::make_index_sequence<transformEnd - Begin>(),
				   std::make_index_sequence<Begin>(),
				   std::make_index_sequence<SIZE - transformEnd>());
		}
	}
};

template<TypeList TL>
using unwrap = decltype(TL)::type;

void fn() {
}

} // namespace arch::utils

#undef TL_ERROR
