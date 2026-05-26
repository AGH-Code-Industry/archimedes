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
	auto pred = []<class T>(TypeList<T>) {
		return std::floating_point<T>;
	};

	auto addPtr = []<class T>(TypeList<T>) {
		return typelist<T*>;
	};

	constexpr auto x0 =
		typelist<int, float, char, void, double, int, float, char, void, double, int, float, char, void, double>;
	constexpr auto x1 = x0.distinct<5>();

	constexpr auto x2 = x0.front();
	using test = unwrap<x2>;

	/*
	template<auto Pred, size_t Begin = 0, size_t Count = -1>
	static consteval auto eraseIf() {
		static_assert(sizeof...(Types) == 0 || Begin < sizeof...(Types), "TypeList::eraseIf: Begin out of range");
		if constexpr (Begin == 0) {
			if constexpr (sizeof...(Types) == 0 || Count == 0) {
				return typelist<Types...>;
			} else if constexpr (Pred(front())) {
				return popFront().eraseIf<Pred, 0, Count - 1>();
			} else {
				return front() + popFront().eraseIf<Pred, 0, Count - 1>();
			}
		} else {
			return sublist<0, Begin>() + popFront<Begin>().eraseIf<Pred, 0, Count>();
		}
	}

	template<template<class T> class TypeTrait, size_t Begin = 0, size_t Count = -1>
	static consteval auto eraseIf() {
		static_assert(sizeof...(Types) == 0 || Begin < sizeof...(Types), "TypeList::eraseIf: Begin out of range");
		if constexpr (Begin == 0) {
			if constexpr (sizeof...(Types) == 0 || Count == 0) {
				return typelist<Types...>;
			} else if constexpr (TypeTrait<typename decltype(front())::type>::value) {
				return popFront().eraseIf<TypeTrait, 0, Count - 1>();
			} else {
				return front() + popFront().eraseIf<TypeTrait, 0, Count - 1>();
			}
		} else {
			return sublist<0, Begin>() + popFront<Begin>().eraseIf<TypeTrait, 0, Count>();
		}
	}

	template<size_t Begin = 0, size_t Count = -1>
	static consteval auto distinct() {
		static_assert(sizeof...(Types) == 0 || Begin < sizeof...(Types), "TypeList::distinct: Begin out of range");
		if constexpr (sizeof...(Types) <= 1 || Begin >= sizeof...(Types)) {
			return typelist<Types...>;
		} else if constexpr (Begin == 0 && Count == (size_t)-1) {
			using FrontT = typename decltype(front())::type;
			constexpr auto pred = []<class T>(TypeList<T> tl) -> bool {
				return TypeList<T>() == TypeList<FrontT>();
			};

			return front() + popFront().eraseIf<pred>().distinct();
		} else {
			return sublist<0, Begin>() + sublist<Begin, Count>().distinct() +
				sublist<Begin + std::min(Count, sizeof...(Types) - Begin)>();
		}
	}

	template<size_t Begin = 0, size_t Count = -1>
	static consteval auto reverse() {
		static_assert(sizeof...(Types) == 0 || Begin < sizeof...(Types), "TypeList::reverse: Begin out of range");
		if constexpr (sizeof...(Types) <= 1 || Begin >= sizeof...(Types)) {
			return typelist<Types...>;
		} else if constexpr (Begin == 0 && Count == (size_t)-1) {
			return popFront().reverse() + front();
		} else {
			return sublist<0, Begin>() + sublist<Begin, Count>().reverse() +
				sublist<Begin + std::min(Count, sizeof...(Types) - Begin)>();
		}
	}

	template<auto Pred, size_t Begin = 0, size_t Count = -1>
	static consteval auto filter() {
		static_assert(sizeof...(Types) == 0 || Begin < sizeof...(Types), "TypeList::filter: Begin out of range");
		return eraseIf<details::notPred<Pred>, Begin, Count>();
	}

	template<template<class> class TypeTrait, size_t Begin = 0, size_t Count = -1>
	static consteval auto filter() {
		static_assert(sizeof...(Types) == 0 || Begin < sizeof...(Types), "TypeList::filter: Begin out of range");
		return eraseIf<details::NotTrait<TypeTrait>::template type, Begin, Count>();
	}

	template<auto Fn, size_t Begin = 0, size_t Count = -1>
	static consteval auto transform() {
		static_assert(sizeof...(Types) == 0 || Begin < sizeof...(Types), "TypeList::transform: Begin out of range");
		if constexpr (sizeof...(Types) == 0 || Begin >= sizeof...(Types)) {
			return typelist<Types...>;
		} else if constexpr (Begin == 0 && Count == (size_t)-1) {
			return Fn(front()) + popFront().transform<Fn>();
		} else {
			return sublist<0, Begin>() + sublist<Begin, Count>().transform<Fn>() +
				sublist<Begin + std::min(Count, sizeof...(Types) - Begin)>();
		}
	}

	template<template<class T> class TypeTrait, size_t Begin = 0, size_t Count = -1>
	static consteval auto transform() {
		static_assert(sizeof...(Types) == 0 || Begin < sizeof...(Types), "TypeList::transform: Begin out of range");
		if constexpr (sizeof...(Types) == 0 || Begin >= sizeof...(Types)) {
			return typelist<Types...>;
		} else if constexpr (Begin == 0 && Count == (size_t)-1) {
			return TypeList<typename TypeTrait<typename decltype(front())::type>::type>() +
				popFront().transform<TypeTrait>();
		} else {
			return sublist<0, Begin>() + sublist<Begin, Count>().transform<TypeTrait>() +
				sublist<Begin + std::min(Count, sizeof...(Types) - Begin)>();
		}
	}
	*/

	// Uncomment each to verify static_assert fires:

	// get out of bounds
	// constexpr auto e0 = typelist<int, float>.get<2>();

	// front on empty
	// constexpr auto e1 = typelist<>.front();

	// back on empty
	// constexpr auto e2 = typelist<>.back();

	// popFront N > size
	// constexpr auto e3 = typelist<int, float>.popFront<3>();

	// popBack N > size
	// constexpr auto e4 = typelist<int, float>.popBack<3>();

	// sublist Begin > size
	// constexpr auto e5 = typelist<int, float>.sublist<3>();

	// insert I > size
	// constexpr auto e6 = typelist<int, float>.insert<3>(typelist<double>);

	// erase Begin >= size
	// constexpr auto e7 = typelist<int, float>.erase<2>();

	// replace Begin >= size
	// constexpr auto e8 = typelist<int, float>.replace<2>(typelist<double>);

	// eraseIf Begin >= size
	// constexpr auto pred = []<class T>(TypeList<T>) -> bool { return true; };
	// constexpr auto e9 = typelist<int, float>.eraseIf<pred, 2>();

	// distinct Begin >= size
	// constexpr auto e10 = typelist<int, float>.distinct<2>();

	// reverse Begin >= size
	// constexpr auto e11 = typelist<int, float>.reverse<2>();

	// filter Begin >= size
	// constexpr auto pred2 = []<class T>(TypeList<T>) -> bool { return true; };
	// constexpr auto e12 = typelist<int, float>.filter<pred2, 2>();

	// transform Begin >= size
	// constexpr auto fn2 = []<class T>(TypeList<T> t) { return t; };
	// constexpr auto e13 = typelist<int, float>.transform<fn2, 2>();
}

} // namespace arch::utils

/**/

namespace arch::utils::tests {

// ============================================================
// Helpers
// ============================================================

template<bool B>
struct Assert {
	static_assert(B, "Test failed");
};

#define TL_ASSERT(...) static_assert((__VA_ARGS__), #__VA_ARGS__)

// ============================================================
// operator== / operator!=
// ============================================================

namespace test_equality {
TL_ASSERT(typelist<> == typelist<>);
TL_ASSERT(typelist<int> == typelist<int>);
TL_ASSERT(typelist<int, float> == typelist<int, float>);
TL_ASSERT(!(typelist<int> == typelist<float>));
TL_ASSERT(typelist<int> != typelist<float>);
TL_ASSERT(!(typelist<int, float> == typelist<float, int>));
TL_ASSERT(typelist<int, float, double> == typelist<int, float, double>);
TL_ASSERT(typelist<int, int> != typelist<int>);
TL_ASSERT(typelist<> != typelist<int>);
TL_ASSERT(typelist<void*, void> != typelist<void, void*>);
} // namespace test_equality

// ============================================================
// size / length / empty
// ============================================================

namespace test_size {
TL_ASSERT(typelist<>.size() == 0);
TL_ASSERT(typelist<int>.size() == 1);
TL_ASSERT(typelist<int, float>.size() == 2);
TL_ASSERT(typelist<int, float, double, char, void>.size() == 5);

TL_ASSERT(typelist<>.length() == 0);
TL_ASSERT(typelist<int, float>.length() == 2);

TL_ASSERT(typelist<>.emtpy() == true);
TL_ASSERT(typelist<int>.emtpy() == false);
TL_ASSERT(typelist<int, float>.emtpy() == false);
} // namespace test_size

// ============================================================
// front / back
// ============================================================

namespace test_front_back {
TL_ASSERT(typelist<int>.front() == typelist<int>);
TL_ASSERT(typelist<int, float>.front() == typelist<int>);
TL_ASSERT(typelist<int, float, double>.front() == typelist<int>);

TL_ASSERT(typelist<int>.back() == typelist<int>);
TL_ASSERT(typelist<int, float>.back() == typelist<float>);
TL_ASSERT(typelist<int, float, double>.back() == typelist<double>);
TL_ASSERT(typelist<void*, char, double>.back() == typelist<double>);
} // namespace test_front_back

// ============================================================
// get
// ============================================================

namespace test_get {
TL_ASSERT(typelist<int>.get<0>() == typelist<int>);
TL_ASSERT(typelist<int, float>.get<0>() == typelist<int>);
TL_ASSERT(typelist<int, float>.get<1>() == typelist<float>);
TL_ASSERT(typelist<int, float, double>.get<0>() == typelist<int>);
TL_ASSERT(typelist<int, float, double>.get<1>() == typelist<float>);
TL_ASSERT(typelist<int, float, double>.get<2>() == typelist<double>);
TL_ASSERT(typelist<void, void*, char, int>.get<3>() == typelist<int>);
} // namespace test_get

// ============================================================
// popFront
// ============================================================

namespace test_popFront {
TL_ASSERT(typelist<int>.popFront() == typelist<>);
TL_ASSERT(typelist<int, float>.popFront() == typelist<float>);
TL_ASSERT(typelist<int, float, double>.popFront() == typelist<float, double>);

// N > 1
TL_ASSERT(typelist<int, float, double>.popFront<2>() == typelist<double>);
TL_ASSERT(typelist<int, float, double>.popFront<3>() == typelist<>);
TL_ASSERT(typelist<int, float, double, char>.popFront<2>() == typelist<double, char>);

// N == 0
TL_ASSERT(typelist<int, float>.popFront<0>() == typelist<int, float>);
} // namespace test_popFront

// ============================================================
// popBack
// ============================================================

namespace test_popBack {
TL_ASSERT(typelist<int>.popBack() == typelist<>);
TL_ASSERT(typelist<int, float>.popBack() == typelist<int>);
TL_ASSERT(typelist<int, float, double>.popBack() == typelist<int, float>);

// N > 1
TL_ASSERT(typelist<int, float, double>.popBack<2>() == typelist<int>);
TL_ASSERT(typelist<int, float, double>.popBack<3>() == typelist<>);
TL_ASSERT(typelist<int, float, double, char>.popBack<2>() == typelist<int, float>);

// N == 0
TL_ASSERT(typelist<int, float>.popBack<0>() == typelist<int, float>);
} // namespace test_popBack

// ============================================================
// append / prepend / operator+
// ============================================================

namespace test_append_prepend {
TL_ASSERT(typelist<int>.append(typelist<float>) == typelist<int, float>);
TL_ASSERT(typelist<>.append(typelist<int>) == typelist<int>);
TL_ASSERT(typelist<int>.append(typelist<>) == typelist<int>);
TL_ASSERT(typelist<int, float>.append(typelist<double, char>) == typelist<int, float, double, char>);

TL_ASSERT(typelist<int>.prepend(typelist<float>) == typelist<float, int>);
TL_ASSERT(typelist<>.prepend(typelist<int>) == typelist<int>);
TL_ASSERT(typelist<int>.prepend(typelist<>) == typelist<int>);
TL_ASSERT(typelist<int, float>.prepend(typelist<double, char>) == typelist<double, char, int, float>);

TL_ASSERT((typelist<int> + typelist<float>) == typelist<int, float>);
TL_ASSERT((typelist<int, float> + typelist<double>) == typelist<int, float, double>);
TL_ASSERT((typelist<> + typelist<int>) == typelist<int>);
TL_ASSERT((typelist<int> + typelist<>) == typelist<int>);
TL_ASSERT((typelist<> + typelist<>) == typelist<>);
} // namespace test_append_prepend

// ============================================================
// sublist
// ============================================================

namespace test_sublist {
// Begin=0, no Count (full list)
TL_ASSERT(typelist<int, float, double>.sublist<0>() == typelist<int, float, double>);

// Begin=0, Count trims tail
TL_ASSERT(typelist<int, float, double>.sublist<0, 2>() == typelist<int, float>);
TL_ASSERT(typelist<int, float, double>.sublist<0, 1>() == typelist<int>);
TL_ASSERT(typelist<int, float, double>.sublist<0, 3>() == typelist<int, float, double>);

// Begin > 0, no Count
TL_ASSERT(typelist<int, float, double>.sublist<1>() == typelist<float, double>);
TL_ASSERT(typelist<int, float, double>.sublist<2>() == typelist<double>);

// Begin > 0, with Count
TL_ASSERT(typelist<int, float, double, char>.sublist<1, 2>() == typelist<float, double>);
TL_ASSERT(typelist<int, float, double, char>.sublist<2, 1>() == typelist<double>);

// Begin == size (empty result)
TL_ASSERT(typelist<int, float>.sublist<2>() == typelist<>);
} // namespace test_sublist

// ============================================================
// insert
// ============================================================

namespace test_insert {
// Insert at front
TL_ASSERT(typelist<int, float>.insert<0>(typelist<double>) == typelist<double, int, float>);

// Insert in middle
TL_ASSERT(typelist<int, float>.insert<1>(typelist<double>) == typelist<int, double, float>);

// Insert at end (append)
TL_ASSERT(typelist<int, float>.insert<2>(typelist<double>) == typelist<int, float, double>);

// Insert multiple
TL_ASSERT(typelist<int, float>.insert<1>(typelist<double, char>) == typelist<int, double, char, float>);

// Insert into empty
TL_ASSERT(typelist<>.insert<0>(typelist<int>) == typelist<int>);

// Insert empty list
TL_ASSERT(typelist<int, float>.insert<1>(typelist<>) == typelist<int, float>);
} // namespace test_insert

// ============================================================
// erase
// ============================================================

namespace test_erase {
// Erase single at front
TL_ASSERT(typelist<int, float, double>.erase<0, 1>() == typelist<float, double>);

// Erase single in middle
TL_ASSERT(typelist<int, float, double>.erase<1, 1>() == typelist<int, double>);

// Erase single at back
TL_ASSERT(typelist<int, float, double>.erase<2, 1>() == typelist<int, float>);

// Erase multiple
TL_ASSERT(typelist<int, float, double, char>.erase<1, 2>() == typelist<int, char>);

// Erase to end (no Count)
TL_ASSERT(typelist<int, float, double>.erase<1>() == typelist<int>);

// Count exceeds remaining — clamps to end
TL_ASSERT(typelist<int, float, double>.erase<1, 10>() == typelist<int>);

// Erase all
TL_ASSERT(typelist<int, float>.erase<0, 2>() == typelist<>);
} // namespace test_erase

// ============================================================
// replace
// ============================================================

namespace test_replace {
// Replace single
TL_ASSERT(typelist<int, float, double>.replace<1, 1>(typelist<char>) == typelist<int, char, double>);

// Replace at front
TL_ASSERT(typelist<int, float, double>.replace<0, 1>(typelist<char>) == typelist<char, float, double>);

// Replace at back
TL_ASSERT(typelist<int, float, double>.replace<2, 1>(typelist<char>) == typelist<int, float, char>);

// Replace with multiple
TL_ASSERT(typelist<int, float>.replace<1, 1>(typelist<double, char>) == typelist<int, double, char>);

// Replace multiple with one
TL_ASSERT(typelist<int, float, double>.replace<0, 2>(typelist<char>) == typelist<char, double>);

// Replace to end (no Count)
TL_ASSERT(typelist<int, float, double>.replace<1>(typelist<char>) == typelist<int, char>);

// Replace with empty (= erase)
TL_ASSERT(typelist<int, float, double>.replace<1, 1>(typelist<>) == typelist<int, double>);
} // namespace test_replace

// ============================================================
// find
// ============================================================

namespace test_find {
// Basic find
TL_ASSERT(typelist<int, float, double>.find(typelist<int>) == 0);
TL_ASSERT(typelist<int, float, double>.find(typelist<float>) == 1);
TL_ASSERT(typelist<int, float, double>.find(typelist<double>) == 2);

// Not found
TL_ASSERT(typelist<int, float, double>.find(typelist<char>) == TypeList<>::npos);

// constexpr auto xd = typelist<int, float, double>.find(typelist<char>);

// Find subsequence
TL_ASSERT(typelist<int, float, double>.find(typelist<float, double>) == 1);
TL_ASSERT(typelist<int, float, double>.find(typelist<int, float>) == 0);

// Find whole list
TL_ASSERT(typelist<int, float>.find(typelist<int, float>) == 0);

// Empty needle — returns Begin
TL_ASSERT(typelist<int, float>.find(typelist<>) == 0);
TL_ASSERT(typelist<int, float>.find<1>(typelist<>) == 1);
TL_ASSERT(typelist<int, float>.find<2>(typelist<>) == 2);

// Begin > 0
TL_ASSERT(typelist<int, float, int>.find<1>(typelist<int>) == 2);

// Begin past end
TL_ASSERT(typelist<int, float>.find<3>(typelist<int>) == TypeList<>::npos);

// Needle larger than haystack
TL_ASSERT(typelist<int>.find(typelist<int, float>) == TypeList<>::npos);

// Find in empty list
TL_ASSERT(typelist<>.find(typelist<int>) == TypeList<>::npos);
TL_ASSERT(typelist<>.find(typelist<>) == 0);

// Duplicate elements — finds first
TL_ASSERT(typelist<int, int, int>.find(typelist<int>) == 0);
} // namespace test_find

// ============================================================
// rfind
// ============================================================

namespace test_rfind {
// Basic rfind — finds last occurrence
TL_ASSERT(typelist<int, float, int>.rfind(typelist<int>) == 2);
TL_ASSERT(typelist<int, float, double>.rfind(typelist<int>) == 0);
TL_ASSERT(typelist<int, float, double>.rfind(typelist<double>) == 2);
TL_ASSERT(typelist<double, double>.rfind(typelist<double>) == 1);

// Single element list
TL_ASSERT(typelist<double>.rfind(typelist<double>) == 0);

// Not found
TL_ASSERT(typelist<int, float>.rfind(typelist<double>) == TypeList<>::npos);

// Empty needle — returns min(Begin, N)
TL_ASSERT(typelist<int, float>.rfind(typelist<>) == 2);
TL_ASSERT(typelist<int, float>.rfind<1>(typelist<>) == 1);
TL_ASSERT(typelist<int, float>.rfind<0>(typelist<>) == 0);
TL_ASSERT(typelist<int, float>.rfind<5>(typelist<>) == 2); // clamped to N

constexpr auto test = typelist<int, float>.rfind(typelist<>);

// Begin clamps to N1 - N2
TL_ASSERT(typelist<int, float, double>.rfind<10>(typelist<float>) == 1);

// Subsequence
TL_ASSERT(typelist<int, float, int, float>.rfind(typelist<int, float>) == 2);

// Needle larger than haystack
TL_ASSERT(typelist<int>.rfind(typelist<int, float>) == TypeList<>::npos);

// All same
TL_ASSERT(typelist<int, int, int>.rfind(typelist<int>) == 2);
} // namespace test_rfind

// ============================================================
// contains / startsWith / endsWith
// ============================================================

namespace test_contains_starts_ends {
TL_ASSERT(typelist<int, float, double>.contains(typelist<float>) == true);
TL_ASSERT(typelist<int, float, double>.contains(typelist<char>) == false);
TL_ASSERT(typelist<int, float, double>.contains(typelist<int, float>) == true);
TL_ASSERT(typelist<int, float, double>.contains(typelist<>) == true);
TL_ASSERT(typelist<>.contains(typelist<>) == true);
TL_ASSERT(typelist<>.contains(typelist<int>) == false);

TL_ASSERT(typelist<int, float, double>.startsWith(typelist<int>) == true);
TL_ASSERT(typelist<int, float, double>.startsWith(typelist<int, float>) == true);
TL_ASSERT(typelist<int, float, double>.startsWith(typelist<float>) == false);
TL_ASSERT(typelist<int, float, double>.startsWith(typelist<>) == true);

TL_ASSERT(typelist<int, float, double>.endsWith(typelist<double>) == true);
TL_ASSERT(typelist<int, float, double>.endsWith(typelist<float, double>) == true);
TL_ASSERT(typelist<int, float, double>.endsWith(typelist<float>) == false);
TL_ASSERT(typelist<int, float, double>.endsWith(typelist<>) == true);
TL_ASSERT(typelist<int>.endsWith(typelist<int, float>) == false); // needle larger
} // namespace test_contains_starts_ends

// ============================================================
// eraseIf (predicate)
// ============================================================

namespace test_eraseIf {
constexpr auto isInt = []<class T>(TypeList<T>) -> bool {
	return std::same_as<T, int>;
};
constexpr auto isPointer = []<class T>(TypeList<T>) -> bool {
	return std::is_pointer_v<T>;
};
constexpr auto alwaysTrue = []<class T>(TypeList<T>) -> bool {
	return true;
};
constexpr auto alwaysFalse = []<class T>(TypeList<T>) -> bool {
	return false;
};

TL_ASSERT(typelist<int, float, int>.eraseIf<isInt>() == typelist<float>);
TL_ASSERT(typelist<int, float, double>.eraseIf<isInt>() == typelist<float, double>);
TL_ASSERT(typelist<float, double>.eraseIf<isInt>() == typelist<float, double>);
TL_ASSERT(typelist<int, int, int>.eraseIf<isInt>() == typelist<>);

TL_ASSERT(typelist<int*, float, double*>.eraseIf<isPointer>() == typelist<float>);

// Count limits how many are erased
TL_ASSERT(typelist<int, int, float>.eraseIf<isInt, 0, 1>() == typelist<int, float>);

// Begin offsets where erasing starts
TL_ASSERT(typelist<int, float, int>.eraseIf<isInt, 1>() == typelist<int, float>);

// alwaysTrue erases all
TL_ASSERT(typelist<int, float, double>.eraseIf<alwaysTrue>() == typelist<>);

// alwaysFalse erases none
TL_ASSERT(typelist<int, float, double>.eraseIf<alwaysFalse>() == typelist<int, float, double>);
} // namespace test_eraseIf

// ============================================================
// eraseIf (type trait)
// ============================================================

namespace test_eraseIf_trait {
TL_ASSERT(typelist<int, float, char>.eraseIf<std::is_integral>() == typelist<float>);
TL_ASSERT(typelist<int*, float*, double>.eraseIf<std::is_pointer>() == typelist<double>);
TL_ASSERT(typelist<float, double>.eraseIf<std::is_integral>() == typelist<float, double>);
TL_ASSERT(typelist<int, char>.eraseIf<std::is_integral>() == typelist<>);
} // namespace test_eraseIf_trait

// ============================================================
// filter (predicate) — inverse of eraseIf
// ============================================================

namespace test_filter {
constexpr auto isFloat = []<class T>(TypeList<T>) -> bool {
	return std::same_as<T, float>;
};

TL_ASSERT(typelist<int, float, double>.filter<isFloat>() == typelist<float>);
TL_ASSERT(typelist<int, double>.filter<isFloat>() == typelist<>);
TL_ASSERT(typelist<float, float>.filter<isFloat>() == typelist<float, float>);
} // namespace test_filter

// ============================================================
// filter (type trait)
// ============================================================

namespace test_filter_trait {
TL_ASSERT(typelist<int, float, char>.filter<std::is_integral>() == typelist<int, char>);
TL_ASSERT(typelist<int*, float*, double>.filter<std::is_pointer>() == typelist<int*, float*>);
TL_ASSERT(typelist<float, double>.filter<std::is_integral>() == typelist<>);
} // namespace test_filter_trait

// ============================================================
// distinct
// ============================================================

namespace test_distinct {
TL_ASSERT(typelist<int, float, double>.distinct() == typelist<int, float, double>);
TL_ASSERT(typelist<int, int, float>.distinct() == typelist<int, float>);
TL_ASSERT(typelist<int, float, int>.distinct() == typelist<int, float>);
TL_ASSERT(typelist<int, int, int>.distinct() == typelist<int>);
TL_ASSERT(typelist<int, float, int, float>.distinct() == typelist<int, float>);
TL_ASSERT(typelist<int>.distinct() == typelist<int>);

// With Begin/Count
TL_ASSERT(typelist<int, int, float, float>.distinct<2>() == typelist<int, int, float>);
TL_ASSERT(typelist<int, int, float, float>.distinct<0, 2>() == typelist<int, float, float>);
} // namespace test_distinct

// ============================================================
// reverse
// ============================================================

namespace test_reverse {
TL_ASSERT(typelist<int, float, double>.reverse() == typelist<double, float, int>);
TL_ASSERT(typelist<int>.reverse() == typelist<int>);
TL_ASSERT(typelist<int, float>.reverse() == typelist<float, int>);
TL_ASSERT(typelist<int, float, double, char>.reverse() == typelist<char, double, float, int>);

// With Begin/Count
TL_ASSERT(typelist<int, float, double>.reverse<1>() == typelist<int, double, float>);
TL_ASSERT(typelist<int, float, double, char>.reverse<1, 2>() == typelist<int, double, float, char>);
} // namespace test_reverse

// ============================================================
// transform (function)
// ============================================================

namespace test_transform {
constexpr auto toPtr = []<class T>(TypeList<T>) {
	return TypeList<T*>();
};
constexpr auto toConst = []<class T>(TypeList<T>) {
	return TypeList<const T>();
};

TL_ASSERT(typelist<int, float>.transform<toPtr>() == typelist<int*, float*>);
TL_ASSERT(typelist<int, float, double>.transform<toPtr>() == typelist<int*, float*, double*>);
TL_ASSERT(typelist<int, float>.transform<toConst>() == typelist<const int, const float>);

// With Begin/Count
TL_ASSERT(typelist<int, float, double>.transform<toPtr, 1>() == typelist<int, float*, double*>);
TL_ASSERT(typelist<int, float, double>.transform<toPtr, 1, 1>() == typelist<int, float*, double>);
} // namespace test_transform

// ============================================================
// transform (type trait)
// ============================================================

namespace test_transform_trait {
TL_ASSERT(typelist<int, float>.transform<std::add_pointer>() == typelist<int*, float*>);
TL_ASSERT(typelist<int, float>.transform<std::add_const>() == typelist<const int, const float>);
TL_ASSERT(typelist<int*, float*>.transform<std::remove_pointer>() == typelist<int, float>);
TL_ASSERT(typelist<const int, const float>.transform<std::remove_const>() == typelist<int, float>);
} // namespace test_transform_trait

// ============================================================
// unwrap
// ============================================================

namespace test_unwrap {
static_assert(std::same_as<unwrap<typelist<int>>, int>);
static_assert(std::same_as<unwrap<typelist<float>>, float>);
static_assert(std::same_as<unwrap<typelist<void*>>, void*>);
} // namespace test_unwrap

// ============================================================
// Composed / chained operations
// ============================================================

namespace test_composed {
// reverse then distinct
TL_ASSERT(typelist<int, float, int>.reverse().distinct() == typelist<int, float>);

// filter then transform
TL_ASSERT(typelist<int, float, char>.filter<std::is_integral>().transform<std::add_pointer>() == typelist<int*, char*>);

// distinct then reverse
TL_ASSERT(typelist<int, float, int, double>.distinct().reverse() == typelist<double, float, int>);

// eraseIf then append
constexpr auto isInt = []<class T>(TypeList<T>) -> bool {
	return std::same_as<T, int>;
};
TL_ASSERT(typelist<int, float, int>.eraseIf<isInt>().append(typelist<double>) == typelist<float, double>);

// insert then erase (round-trip)
TL_ASSERT(typelist<int, float>.insert<1>(typelist<double>).erase<1, 1>() == typelist<int, float>);

// replace then reverse
TL_ASSERT(typelist<int, float, double>.replace<1, 1>(typelist<char>).reverse() == typelist<double, char, int>);

// find after transform
TL_ASSERT(typelist<int, float>.transform<std::add_pointer>().find(typelist<int*>) == 0);

// sublist then reverse
TL_ASSERT(typelist<int, float, double, char>.sublist<1, 3>().reverse() == typelist<char, double, float>);

// popFront + popBack round-trip
TL_ASSERT((typelist<int> + typelist<int, float, double>.popFront().popBack()) == typelist<int, float>);
} // namespace test_composed

} // namespace arch::utils::tests

/**/

#undef TL_ERROR
