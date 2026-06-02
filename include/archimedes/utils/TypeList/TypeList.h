#pragma once

#include "TypeList.hpp"

#include "TypeListFwd.h"

// The below implementations heavily rely on niche std::index_sequence,
// which acts in this context as "for" loop.
// This eliminates most of recursive templates, which are expensive

// Helper for static_asserts, because static_assert still fires, even if inside inactive if constexpr branch
// UNLESS it relies on template, in this case alwaysFalse<size_t>
#define TL_ERROR(msg) static_assert(details::alwaysFalse<SIZE>, msg)

namespace arch::utils {

/// @brief Compares two typelists
template<class... Types, class... Types2>
static consteval auto operator==(TypeList<Types...>, TypeList<Types2...>) {
	return std::same_as<TypeList<Types...>, TypeList<Types2...>>;
}

/// @brief Compares two typelists
template<class... Types, class... Types2>
static consteval auto operator!=(TypeList<Types...>, TypeList<Types2...>) {
	return !std::same_as<TypeList<Types...>, TypeList<Types2...>>;
}

template<class... Types>
struct TypeList: details::SingleTypeAlias<sizeof...(Types) == 1, Types...> {
private:
	// Helpers
	using This = TypeList<Types...>;
	static inline constexpr size_t SIZE = sizeof...(Types);

	template<size_t I>
	using Get = std::conditional_t<SIZE != 0, typename details::TLGet<I, Types...>::type, void>;

public:

	/// @brief Value representing "end of list" or "not found"
	static inline constexpr size_t npos = (size_t)-1;

	/// @brief Checks if typelist is empty
	static consteval auto emtpy() { return SIZE == 0; }

	/// @brief Returns typelist size
	static consteval auto size() { return SIZE; }

	/// @brief Returns typelist size
	static consteval auto length() { return SIZE; }

	/// @brief Returns typelist with I-th type
	/// @tparam I - index of type to get
	template<size_t I>
	static consteval auto get() {
		if constexpr (SIZE == 0 || I >= SIZE) {
			TL_ERROR("TypeList::get: index out of bounds");
			return typelist<>;
		} else {
			return typelist<Get<I>>;
		}
	}

	/// @brief Returns typelist with the first type
	static consteval auto front() {
		if constexpr (SIZE == 0) {
			TL_ERROR("TypeList::front: empty typelist");
			return typelist<>;
		} else {
			return get<0>();
		}
	}

	/// @brief Returns typelist with the last type
	static consteval auto back() {
		if constexpr (SIZE < 2) {
			static_assert(SIZE != 0, "TypeList::back: empty typelist");
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
			TL_ERROR("TypeList::popFront: empty typelist");
			return typelist<>;
		} else if constexpr (SIZE < N) {
			TL_ERROR("TypeList::popFront: N bigger that typelist.size()");
			return typelist<>;
		} else {
			return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typelist<Get<Indexes + N>...>; // move i by N
			}(std::make_index_sequence<SIZE - N>()); // for i in range(SIZE - N)
		}
	}

	/// @brief Removes N last types
	/// @tparam N - how many types to remove
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
			TL_ERROR("TypeList::sublist: Begin bigger that typelist.size()");
			return typelist<>;
		} else {
			constexpr auto sublistSize = std::min(Count, SIZE - Begin);

			return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
				return typelist<Get<Indexes + Begin>...>; // move i by Begin
			}(std::make_index_sequence<sublistSize>()); // for i in range(sublistSize)
		}
	}

	/// @brief Inserts types at given position
	/// @tparam Pos - position to insert types at
	/// @param other - typelist to insert
	template<size_t Pos, class... Types2>
	static consteval auto insert(TypeList<Types2...> other) {
		if constexpr (Pos > SIZE) {
			TL_ERROR("Typelist::insert: Pos out of range");
			return typelist<>;
		} else {
			constexpr auto rightSize = SIZE - Pos;

			return []<size_t... Head, size_t... Tail>(std::index_sequence<Head...>, std::index_sequence<Tail...>) {
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

			return []<size_t... Head, size_t... Tail>(std::index_sequence<Head...>, std::index_sequence<Tail...>) {
				return typelist<Get<Head>..., Get<Tail + rightBegin>...>;
			}(std::make_index_sequence<Begin>(), std::make_index_sequence<rightSize>());
		}
	}

	/// @brief Appends types
	/// @param other - typelist to append
	template<class... Types2>
	static consteval auto append(TypeList<Types2...> other) {
		return typelist<Types..., Types2...>;
	}

	/// @brief Prepends types
	/// @param other - typelist to prepend
	template<class... Types2>
	static consteval auto prepend(TypeList<Types2...>) {
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

	/// @brief Finds first occurence of given type sequence in given range
	/// @tparam Begin - beginning of range to search
	/// @tparam Count - max length of range to search
	/// @param other - typelist to find
	template<size_t Begin = 0, size_t Count = npos, class... Types2>
	static consteval size_t find(TypeList<Types2...>) {
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
				return []<size_t... Indexes>(std::index_sequence<Indexes...>) {
					size_t result = npos;
					(... ||
					 ((result == npos && sublist<Indexes + Begin, other.size()>() == other) ?
						  (result = Indexes + Begin, true) :
						  false));
					return result;
				}(std::make_index_sequence<searchEnd - Begin>());
			}
		}
	}

	/// @brief Finds last occurence of given type sequence in given range
	/// @tparam Begin - beginning of range to search
	/// @tparam Count - max length of range to search
	/// @param other - typelist to find
	template<size_t Begin = npos, size_t Count = npos, class... Types2>
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

	/// @brief Checks if typelist contains given type sequence
	/// @param other - typelist to find
	template<class... Types2>
	static consteval auto contains(TypeList<Types2...> other) {
		return find(other) != npos;
	}

	/// @brief Checks if typelist contains all types from sequence
	/// @param other - typelist with types to find
	template<class... Types2>
	static consteval auto containsAll(TypeList<Types2...>) {
		return (... && contains(TypeList<Types2>()));
	}

	/// @brief Checks if typelist starts with given type sequence
	/// @param other - typelist to find
	template<class... Types2>
	static consteval auto startsWith(TypeList<Types2...> other) {
		return find(other) == 0;
	}

	/// @brief Checks if typelist ends with given type sequence
	/// @param other - typelist to find
	template<class... Types2>
	static consteval auto endsWith(TypeList<Types2...> other) {
		return other.size() <= size() && rfind(other) == size() - other.size();
	}

	/// @brief Erases types for which predicate returned true
	/// @tparam Pred - predicate TypeList<T> -> bool
	/// @tparam Begin - beginning of range to erase
	/// @tparam Count - max length of range to erase
	template<auto Pred, size_t Begin = 0, size_t Count = npos>
	static consteval auto eraseIf() {
		if constexpr (SIZE != 0 && Begin >= SIZE) {
			TL_ERROR("TypeList::eraseIf: Begin out of range");
		} else if constexpr (SIZE == 0 || Count == 0) {
			return This();
		} else {
			constexpr auto searchEnd = Count >= SIZE - Begin ? SIZE : std::min(Begin + Count, SIZE);

			return []<size_t... Indexes>(std::index_sequence<Indexes...> seq) {
				constexpr auto keepIndexes = utils::filterIntegerSequence<
					(Indexes < Begin || Indexes >= searchEnd || !Pred(typelist<Get<Indexes>>))... // mask types to keep
					>(seq);

				return []<size_t... KIndexes>(std::index_sequence<KIndexes...>) {
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
			TL_ERROR("TypeList::eraseIf: Begin out of range");
		} else if constexpr (SIZE == 0 || Count == 0) {
			return This();
		} else {
			constexpr auto searchEnd = Count >= SIZE - Begin ? SIZE : std::min(Begin + Count, SIZE);

			return []<size_t... Indexes>(std::index_sequence<Indexes...> seq) {
				constexpr auto keepIndexes = utils::filterIntegerSequence<
					(Indexes < Begin || Indexes >= searchEnd || !TypeTrait<Get<Indexes>>::value)... // mask types to
																									// keep
					>(seq);

				return []<size_t... KIndexes>(std::index_sequence<KIndexes...>) {
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
			TL_ERROR("TypeList::distinct: Begin out of range");
		} else if constexpr (SIZE == 0 || Count == 0) {
			return This{};
		} else {
			constexpr auto searchEnd = (Count >= SIZE - Begin) ? SIZE : std::min(Begin + Count, SIZE);

			return []<size_t... Indexes>(std::index_sequence<Indexes...> seq) {
				constexpr auto keepIndexes = utils::filterIntegerSequence<
					(Indexes < Begin || Indexes >= searchEnd ||
					 find<Begin, searchEnd - Begin>(typelist<Get<Indexes>>) == Indexes)... // mask types to keep
					>(seq);

				return []<size_t... KIndexes>(std::index_sequence<KIndexes...>) {
					return typelist<Get<KIndexes>...>;
				}(keepIndexes); // return types at keepIndexes...
			}(std::make_index_sequence<SIZE>());
		}
	}

	/// @brief Retains types for which predicate returned true, erasing the rest
	/// @tparam Pred - predicate TypeList<T> -> bool
	/// @tparam Begin - beginning of range to filter
	/// @tparam Count - max length of range to filter
	template<auto Pred, size_t Begin = 0, size_t Count = npos>
	static consteval auto filter() {
		static_assert(SIZE == 0 || Begin <= SIZE, "TypeList::filter: Begin out of range");
		return eraseIf<[](auto tl) { return !Pred(tl); }, Begin, Count>();
	}

	/// @brief Retains types for which predicate returned true, erasing the rest
	/// @tparam Pred - type trait T -> bool
	/// @tparam Begin - beginning of range to filter
	/// @tparam Count - max length of range to filter
	template<template<class T> class TypeTrait, size_t Begin = 0, size_t Count = npos>
	static consteval auto filter() {
		static_assert(SIZE == 0 || Begin <= SIZE, "TypeList::filter: Begin out of range");
		return eraseIf<details::NotTrait<TypeTrait>::template type, Begin, Count>();
	}

	/// @brief Reverses typelist in given range
	/// @tparam Begin - beginning of range to reverse
	/// @tparam Count - max length of range to reverse
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

	/// @brief Transforms types using given function
	/// @tparam Fn - function TypeList<T1> -> TypeList<T2>
	/// @tparam Begin - beginning of range to transform
	/// @tparam Count - max length of range to transform
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

	/// @brief Transforms types using given type trait
	/// @tparam Pred - type trait T1 -> T2
	/// @tparam Begin - beginning of range to transform
	/// @tparam Count - max length of range to transform
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

/// @brief Unwraps single typed typelists
/// @details Meant to be used as the last operation in typelist manipulations
/// @tparam TL - typelist to unwrap
template<TypeList TL>
using unwrapTL = decltype(TL)::type;

void fn() {
	constexpr auto test = typelist<int, float, char>.containsAll(typelist<int>);
}

} // namespace arch::utils

#undef TL_ERROR
