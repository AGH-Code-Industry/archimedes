#pragma once

namespace arch::traits {

template<template<class...> class Trait1, template<class...> class Trait2>
struct And {
	template<class... Ts>
	struct type {
		static constexpr bool value = Trait1<Ts...>::value && Trait2<Ts...>::value;
	};
};

} // namespace arch::traits
