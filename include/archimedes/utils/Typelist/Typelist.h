#pragma once

#include "Typelist.hpp"

#include "TypelistFwd.h"
#include <archimedes/utils/FilterIntegerSequence.h>

// The below implementations heavily rely on niche std::index_sequence,
// which acts in this context as "for" loop.
// This eliminates most of recursive templates, which are expensive
// Example:
// This lambda:
// [&]<size_t... i>(std::index_sequence<i...>) {
//     return typelist<Get<i>...>;
// }(std::make_index_sequence<N>());
// Is analogous to:
// typelist(Get(i) for i in range(N))

// Helper for static_asserts, because static_assert still fires, even if inside inactive if constexpr branch
// UNLESS it relies on template, in this case alwaysFalse<size_t>
#define TL_ERROR(msg) static_assert(details::alwaysFalse<SIZE>, msg)

namespace arch::utils {

/// @brief Compares two typelists
template<class... Types, class... Types2>
static consteval auto operator==(Typelist<Types...>, Typelist<Types2...>) {
	return std::is_same_v<Typelist<Types...>, Typelist<Types2...>>;
}

/// @brief Compares two typelists
template<class... Types, class... Types2>
static consteval auto operator!=(Typelist<Types...>, Typelist<Types2...>) {
	return !std::is_same_v<Typelist<Types...>, Typelist<Types2...>>;
}

template<class... Types>
struct Typelist: details::SingleTypeAlias<sizeof...(Types) == 1, Types...> {
private:
	// Helpers
	using This = Typelist<Types...>;
	static inline constexpr size_t SIZE = sizeof...(Types);

	template<size_t I>
	using Get = std::conditional_t<SIZE != 0, typename details::TLGet<I, Types...>::type, void>;

public:

	/// @brief Value representing "end of list" or "not found"
	static inline constexpr size_t npos = (size_t)-1;

	/// @brief Checks if typelist is empty
	static consteval auto emtpy() {
		return SIZE == 0;
	}

	/// @brief Returns typelist size
	static consteval auto size() {
		return SIZE;
	}

	/// @brief Returns typelist size
	static consteval auto length() {
		return SIZE;
	}

	/// @brief Returns typelist with I-th type
	/// @tparam I - index of type to get
	template<size_t I>
	static consteval auto get() {
		if constexpr (SIZE == 0 || I >= SIZE) {
			TL_ERROR("Typelist::get: index out of bounds");
			return typelist<>;
		} else {
			return typelist<Get<I>>;
		}
	}

	/// @brief Returns typelist with the first type
	static consteval auto front() {
		if constexpr (SIZE == 0) {
			TL_ERROR("Typelist::front: empty typelist");
			return typelist<>;
		} else {
			return get<0>();
		}
	}

	/// @brief Returns typelist with the last type
	static consteval auto back() {
		if constexpr (SIZE < 2) {
			static_assert(SIZE != 0, "Typelist::back: empty typelist");
			return This();
		} else {
			return get<SIZE - 1>();
		}
	}

	/// @brief Removes N first types
	/// @tparam N - how many types to remove
	template<size_t N = 1>
	static consteval auto popFront() {
		if constexpr (SIZE == 0) {
			TL_ERROR("Typelist::popFront: empty typelist");
			return typelist<>;
		} else if constexpr (SIZE < N) {
			TL_ERROR("Typelist::popFront: N bigger that typelist.size()");
			return typelist<>;
		} else {
			return [&]<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typelist<Get<Indexes + N>...>; // move i by N
			}(std::make_index_sequence<SIZE - N>()); // for i in range(SIZE - N)
		}
	}

	/// @brief Removes N last types
	/// @tparam N - how many types to remove
	template<size_t N = 1>
	static consteval auto popBack() {
		if constexpr (SIZE == 0) {
			TL_ERROR("Typelist::popBack: empty typelist");
			return typelist<>;
		} else if constexpr (SIZE < N) {
			TL_ERROR("Typelist::popBack: N bigger that typelist.size()");
			return typelist<>;
		} else {
			return [&]<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typelist<Get<Indexes>...>; // get elems in range [0, SIZE - N)
			}(std::make_index_sequence<SIZE - N>()); // for i in range(SIZE - N)
		}
	}

	/// @brief Returns a sublist
	/// @tparam Begin - beginning of new list
	/// @tparam Count - max length of new list
	template<size_t Begin = 0, size_t Count = npos>
	static consteval auto sublist() {
		if constexpr (Begin > SIZE) {
			TL_ERROR("Typelist::sublist: Begin bigger that typelist.size()");
			return typelist<>;
		} else {
			constexpr auto sublistSize = std::min(Count, SIZE - Begin);

			return [&]<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typelist<Get<Indexes + Begin>...>; // move i by Begin
			}(std::make_index_sequence<sublistSize>()); // for i in range(sublistSize)
		}
	}

	/// @brief Inserts types at given position
	/// @tparam Pos - position to insert types at
	/// @param other - typelist to insert
	template<size_t Pos, class... Types2>
	static consteval auto insert(Typelist<Types2...> other) {
		if constexpr (Pos > SIZE) {
			TL_ERROR("Typelist::insert: Pos out of range");
			return typelist<>;
		} else {
			constexpr auto rightSize = SIZE - Pos;

			return [&]<size_t... Head, size_t... Tail>(std::index_sequence<Head...>, std::index_sequence<Tail...>) {
				return typelist<Get<Head>..., Types2..., Get<Tail + Pos>...>; // move j by Pos
			}(std::make_index_sequence<Pos>(), std::make_index_sequence<rightSize>()); // for i in range(Pos) &
																					   // for j in range(rightSize)
		}
	}

	/// @brief Removes types in given range
	/// @tparam Begin - beginning of range to remove
	/// @tparam Count - max length of range to remove
	template<size_t Begin = 0, size_t Count = npos>
	static consteval auto erase() {
		if constexpr (Begin > SIZE) {
			TL_ERROR("Typelist::erase: Begin out of range");
			return typelist<>;
		} else {
			constexpr auto toErase = std::min(Count, SIZE - Begin);
			constexpr auto rightBegin = Begin + toErase;
			constexpr auto rightSize = SIZE - rightBegin;

			return [&]<size_t... Head, size_t... Tail>(std::index_sequence<Head...>, std::index_sequence<Tail...>) {
				return typelist<Get<Head>..., Get<Tail + rightBegin>...>; // move j by rightBegin
			}(std::make_index_sequence<Begin>(), std::make_index_sequence<rightSize>()); // for i in range(Begin) &
																						 // for j in range(rightSize)
		}
	}

	/// @brief Appends types
	/// @param other - typelist to append
	template<class... Types2>
	static consteval auto append(Typelist<Types2...> other) {
		return typelist<Types..., Types2...>;
	}

	/// @brief Prepends types
	/// @param other - typelist to prepend
	template<class... Types2>
	static consteval auto prepend(Typelist<Types2...>) {
		return typelist<Types2..., Types...>;
	}

	/// @brief Concatenates typelists
	/// @param others... - typelist to concatenate
	template<class... Others>
	static consteval auto cat(Others... others) {
		return (This() + ... + others); // fold add
	}

	/// @brief Replaces types in given range
	/// @tparam Begin - beginning of range to replace
	/// @tparam Count - max length of range to replace
	/// @param other - typelist to insert
	template<size_t Begin = 0, size_t Count = npos, class... Types2>
	static consteval auto replace(Typelist<Types2...> other) {
		if constexpr (Begin > SIZE) {
			TL_ERROR("Typelist::replace: Begin out of range");
			return typelist<>;
		} else {
			constexpr auto toErase = std::min(Count, SIZE - Begin);
			constexpr auto rightBegin = Begin + toErase;
			constexpr auto rightSize = SIZE - rightBegin;

			return [&]<size_t... Head, size_t... Tail>(std::index_sequence<Head...>, std::index_sequence<Tail...>) {
				return typelist<Get<Head>..., Types2..., Get<Tail + rightBegin>...>; // move j by rightBegin
			}(std::make_index_sequence<Begin>(), std::make_index_sequence<rightSize>()); // for i in range(Begin) &
																						 // for j in range(rightSize)
		}
	}

	/// @brief Finds first occurence of given type sequence in given range
	/// @tparam Begin - beginning of range to search
	/// @tparam Count - max length of range to search
	/// @param other - typelist to find
	template<size_t Begin = 0, size_t Count = npos, class... Types2>
	static consteval size_t find(Typelist<Types2...>) {
		constexpr auto other = typelist<Types2...>;

		if constexpr (other.size() == 0) {
			return (Begin <= SIZE) ? Begin : 0;
		} else if constexpr (Begin >= SIZE || other.size() > SIZE) {
			return npos;
		} else {
			constexpr auto searchEnd = std::min(Count <= SIZE - Begin ? Begin + Count : SIZE, SIZE - other.size() + 1);

			if constexpr (Begin >= searchEnd) {
				return npos;
			} else {
				return [&]<size_t... Indexes>(std::index_sequence<Indexes...>) {
					size_t result = npos;
					(... ||
					 ((result == npos && sublist<Indexes + Begin, other.size()>() == other) ?
						  (result = Indexes + Begin, true) : // move i by Begin
						  false)); // fold search, short circuits when found
					return result;
				}(std::make_index_sequence<searchEnd - Begin>()); // for i in range(searchEnd - Begin)
			}
		}
	}

	/// @brief Finds last occurence of given type sequence in given range
	/// @tparam Begin - beginning of range to search
	/// @tparam Count - max length of range to search
	/// @param other - typelist to find
	template<size_t Begin = npos, size_t Count = npos, class... Types2>
	static consteval size_t rfind(Typelist<Types2...>) {
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
				return [&]<size_t... Indexes>(std::index_sequence<Indexes...>) {
					size_t result = npos;
					(... ||
					 ((result == npos && sublist<searchBegin - Indexes, other.size()>() == other) ?
						  (result = searchBegin - Indexes, true) : // reverse iteration range
						  false));
					return result;
				}(std::make_index_sequence<searchBegin - searchEnd + 1>()); // for i in range(searchBegin - searchEnd +
																			// 1)
			}
		}
	}

	/// @brief Checks if typelist contains given type sequence
	/// @param other - typelist to find
	template<class... Types2>
	static consteval auto contains(Typelist<Types2...> other) {
		return find(other) != npos;
	}

	/// @brief Checks if typelist contains all types from sequence
	/// @param other - typelist with types to find
	template<class... Types2>
	static consteval auto containsAll(Typelist<Types2...>) {
		constexpr auto containsSingle = [&]<class T>(Typelist<T>) {
			return (std::is_same_v<T, Types> || ...);
		};
		return (... && containsSingle(typelist<Types2>)); // fold search
	}

	/// @brief Checks if typelist contains all types from sequence
	/// @param other - typelist with types to find
	template<class... Types2>
	static consteval auto containsAny(Typelist<Types2...>) {
		constexpr auto containsSingle = [&]<class T>(Typelist<T>) {
			return (std::is_same_v<T, Types> || ...);
		};
		return (... || containsSingle(typelist<Types2>)); // fold search
	}

	/// @brief Checks if typelist starts with given type sequence
	/// @param other - typelist to find
	template<class... Types2>
	static consteval auto startsWith(Typelist<Types2...> other) {
		return find(other) == 0;
	}

	/// @brief Checks if typelist ends with given type sequence
	/// @param other - typelist to find
	template<class... Types2>
	static consteval auto endsWith(Typelist<Types2...> other) {
		return other.size() <= size() && rfind(other) == size() - other.size();
	}

	/// @brief Erases types for which predicate returned true
	/// @tparam Pred - predicate Typelist<T> -> bool
	/// @tparam Begin - beginning of range to erase
	/// @tparam Count - max length of range to erase
	template<auto Pred, size_t Begin = 0, size_t Count = npos>
	static consteval auto eraseIf() {
		if constexpr (SIZE != 0 && Begin >= SIZE) {
			TL_ERROR("Typelist::eraseIf: Begin out of range");
		} else if constexpr (SIZE == 0 || Count == 0) {
			return This();
		} else {
			constexpr auto searchEnd = Count >= SIZE - Begin ? SIZE : std::min(Begin + Count, SIZE);

			return [&]<size_t... Indexes>(std::index_sequence<Indexes...> seq) {
				constexpr auto keepIndexes = utils::filterIntegerSequence<
					(Indexes < Begin || Indexes >= searchEnd || !Pred(typelist<Get<Indexes>>))... // mask types to keep
					>(seq);

				return [&]<size_t... KIndexes>(std::index_sequence<KIndexes...>) {
					return typelist<Get<KIndexes>...>;
				}(keepIndexes); // return types at keepIndexes...
			}(std::make_index_sequence<SIZE>());
		}
	}

	/// @brief Erases types for which predicate returned true
	/// @tparam Pred - type trait T -> bool
	/// @tparam Begin - beginning of range to erase
	/// @tparam Count - max length of range to erase
	template<template<class T> class TypeTrait, size_t Begin = 0, size_t Count = npos>
	static consteval auto eraseIf() {
		if constexpr (SIZE != 0 && Begin >= SIZE) {
			TL_ERROR("Typelist::eraseIf: Begin out of range");
		} else if constexpr (SIZE == 0 || Count == 0) {
			return This();
		} else {
			constexpr auto searchEnd = Count >= SIZE - Begin ? SIZE : std::min(Begin + Count, SIZE);

			return [&]<size_t... Indexes>(std::index_sequence<Indexes...> seq) {
				constexpr auto keepIndexes = utils::filterIntegerSequence<
					(Indexes < Begin || Indexes >= searchEnd || !TypeTrait<Get<Indexes>>::value)... // mask types to
																									// keep
					>(seq);

				return [&]<size_t... KIndexes>(std::index_sequence<KIndexes...>) {
					return typelist<Get<KIndexes>...>;
				}(keepIndexes); // return types at keepIndexes...
			}(std::make_index_sequence<SIZE>());
		}
	}

	/// @brief Erases duplicate types in given range
	/// @tparam Begin - beginning of range to make distinct
	/// @tparam Count - max length of range to distinct
	template<size_t Begin = 0, size_t Count = npos>
	static consteval auto distinct() {
		if constexpr (SIZE != 0 && Begin >= SIZE) {
		} else if constexpr (SIZE == 0 || Count == 0) {
			return This{};
		} else {
			constexpr auto searchEnd = (Count >= SIZE - Begin) ? SIZE : std::min(Begin + Count, SIZE);

			return [&]<size_t... Indexes>(std::index_sequence<Indexes...> seq) {
				constexpr auto keepIndexes = utils::filterIntegerSequence<
					(Indexes < Begin || Indexes >= searchEnd ||
					 find<Begin, searchEnd - Begin>(typelist<Get<Indexes>>) == Indexes)... // mask types to keep
					>(seq);

				return [&]<size_t... KIndexes>(std::index_sequence<KIndexes...>) {
					return typelist<Get<KIndexes>...>;
				}(keepIndexes); // return types at keepIndexes...
			}(std::make_index_sequence<SIZE>());
		}
	}

	/// @brief Retains types for which predicate returned true, erasing the rest
	/// @tparam Pred - predicate Typelist<T> -> bool
	/// @tparam Begin - beginning of range to filter
	/// @tparam Count - max length of range to filter
	template<auto Pred, size_t Begin = 0, size_t Count = npos>
	static consteval auto filter() {
		return eraseIf<
			[&](auto tl) {
				return !Pred(tl);
			},
			Begin,
			Count>();
	}

	/// @brief Retains types for which predicate returned true, erasing the rest
	/// @tparam Pred - type trait T -> bool
	/// @tparam Begin - beginning of range to filter
	/// @tparam Count - max length of range to filter
	template<template<class T> class TypeTrait, size_t Begin = 0, size_t Count = npos>
	static consteval auto filter() {
		return eraseIf<details::NotTrait<TypeTrait>::template type, Begin, Count>();
	}

	/// @brief Reverses typelist in given range
	/// @tparam Begin - beginning of range to reverse
	/// @tparam Count - max length of range to reverse
	template<size_t Begin = 0, size_t Count = npos>
	static consteval auto reverse() {
		if constexpr (Begin >= SIZE) {
			return typelist<>;
		} else {
			constexpr auto reverseEnd = (Count >= SIZE - Begin) ? SIZE : std::min(Begin + Count, SIZE);
			constexpr auto getIndex = [&](size_t i) { // reverse i in range [Begin, Begin + Count)
				if (Begin <= i && i < reverseEnd) {
					return (reverseEnd - 1) - (i - Begin);
				}
				return i;
			};

			return [&]<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typelist<Get<getIndex(Indexes)>...>;
			}(std::make_index_sequence<SIZE>());
		}
	}

	/// @brief Transforms types using given function
	/// @tparam Fn - function Typelist<T1> -> Typelist<T2>
	/// @tparam Begin - beginning of range to transform
	/// @tparam Count - max length of range to transform
	template<auto Fn, size_t Begin = 0, size_t Count = npos>
	static consteval auto transform() {
		if constexpr (Begin >= SIZE) {
			return typelist<>;
		} else {
			constexpr auto transformEnd = (Count >= SIZE - Begin) ? SIZE : std::min(Begin + Count, SIZE);

			return [&]<size_t... Indexes, size_t... Head, size_t... Tail>(
					   std::index_sequence<Indexes...>,
					   std::index_sequence<Head...>,
					   std::index_sequence<Tail...>
				   ) {
				return typelist<Get<Head>...>.cat( // j
					Fn(typelist<Get<Indexes + Begin>>)..., // move i by Begin
					typelist<Get<Tail + transformEnd>...> // move k by transformEnd
				);
			}(std::make_index_sequence<transformEnd - Begin>(), // for i in range(transformEnd - Begin) &
				   std::make_index_sequence<Begin>(), // for j in range(Begin) &
				   std::make_index_sequence<SIZE - transformEnd>()); // for k in range(SIZE - transformEnd)
		}
	}

	/// @brief Transforms types using given type trait
	/// @tparam Pred - type trait T1 -> T2
	/// @tparam Begin - beginning of range to transform
	/// @tparam Count - max length of range to transform
	template<template<class T> class TypeTrait, size_t Begin = 0, size_t Count = npos>
	static consteval auto transform() {
		if constexpr (Begin >= SIZE) {
			return typelist<>;
		} else {
			constexpr auto transformEnd = (Count >= SIZE - Begin) ? SIZE : std::min(Begin + Count, SIZE);

			return [&]<size_t... Indexes, size_t... Head, size_t... Tail>(
					   std::index_sequence<Indexes...>,
					   std::index_sequence<Head...>,
					   std::index_sequence<Tail...>
				   ) {
				return typelist<Get<Head>...>.cat( // j
					typelist<typename TypeTrait<Get<Indexes + Begin>>::type>..., // move i by Begin
					typelist<Get<Tail + transformEnd>...> // move k by transformEnd
				);
			}(std::make_index_sequence<transformEnd - Begin>(), // for i in range(transformEnd - Begin) &
				   std::make_index_sequence<Begin>(), // for j in range(Begin) &
				   std::make_index_sequence<SIZE - transformEnd>()); // for k in range(SIZE - transformEnd)
		}
	}
};

/// @brief Unwraps single typed typelists
/// @details Meant to be used as the last operation in typelist manipulations
/// @tparam TL - typelist to unwrap
template<Typelist TL>
using getType = decltype(TL)::type;

} // namespace arch::utils

#undef TL_ERROR
