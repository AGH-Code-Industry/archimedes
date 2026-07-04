#include "../IsEntity.h"
#include "GeneralView.h"
#include <archimedes/utils/IsApplicable.h>

#define TEMPLATE_IE                                \
	template<class... Includes, class... Excludes> \
	requires(sizeof...(Includes) != 0 || sizeof...(Excludes) != 0)

#define VIEW_IE View<Typelist<Includes...>, Typelist<Excludes...>>

namespace arch::ecs {

namespace _details {

// helper lambda to obtain ComponentPool type
constexpr auto cpoolCast = []<class T>(Typelist<T> c) {
	if constexpr (typelist<T>.apply<std::is_const>()) {
		// const T => const CPool<T>*
		return typelist<const ComponentPool<std::remove_const_t<T>>*>;
	} else {
		// T => CPool<T>*
		return typelist<ComponentPool<T>*>;
	}
};

} // namespace _details

TEMPLATE_IE
VIEW_IE::View(Domain& domain) noexcept {
	_cpools = { (_details::CommonComponentPool*)domain._tryGetCPool<Includes>()... };
	_cpoolsExcl = { (const _details::CommonComponentPool*)domain._tryGetCPool<Excludes>()... };
	_minCpoolIdx = [&] {
		u32 result = 0;
		for (auto i = 0u; i != _cpools.size(); ++i) {
			if (!_cpools[i] || _cpools[i]->count() == 0) {
				// one of cpools is empty or nonexisting
				return (u32)-1;
			}
			if (_cpools[i]->count() < _cpools[result]->count()) {
				result = i;
			}
		}
		return result;
	}();
}

TEMPLATE_IE
VIEW_IE::~View() noexcept {
	this->_cpools = {};
	this->_cpoolsExcl = {};
	this->_minCpoolIdx = {};
}

TEMPLATE_IE
VIEW_IE::Iterator VIEW_IE::begin() const noexcept {
	return Iterator(*this, false);
}

TEMPLATE_IE
VIEW_IE::Iterator VIEW_IE::end() const noexcept {
	return Iterator(*this, true);
}

TEMPLATE_IE
consteval auto VIEW_IE::_nonFlags() {
	return includes.eraseIf<[]<class T>(Typelist<T>) {
		return _details::ComponentTraits<T>::flag;
	}>();
}

TEMPLATE_IE
consteval auto VIEW_IE::_availableComponents() {
	constexpr auto nonFlags = _nonFlags();
	constexpr auto nonConstAsConst = nonFlags.eraseIf<std::is_const>().transform<std::add_const>();

	return nonFlags + nonConstAsConst;
}

TEMPLATE_IE
void VIEW_IE::forEach(auto&& fn) {
	if (_minCpoolIdx == (u32)-1) { // empty view
		return;
	}

	constexpr auto nonFlags = _nonFlags();

	using Traits = utils::CallableTraits<decltype(fn)>;
	if constexpr (Traits::isCallable) { // non-template callable
		constexpr auto args = Traits::args;
		constexpr bool entityFirst = args.front().apply<_details::IsEntity>();
		constexpr auto wanted = args.popFront<entityFirst>() // remove first if entity
									.transform<std::remove_reference>(); // remove references

		constexpr auto available = _availableComponents();

		static_assert(available.containsAll(wanted), "wanted components exceed available");

		_forEach<entityFirst>(std::forward<decltype(fn)>(fn), wanted);
	} else if constexpr (utils::
							 isApplicableV<decltype(fn), decltype(nonFlags.transform<std::add_lvalue_reference>())>) {
		// template matching of available
		_forEach<false>(std::forward<decltype(fn)>(fn), nonFlags);
	} else if constexpr (utils::isApplicableV<
							 decltype(fn),
							 decltype(typelist<Entity> + nonFlags.transform<std::add_lvalue_reference>())>) {
		// template matching of entity + available
		_forEach<true>(std::forward<decltype(fn)>(fn), nonFlags);
	} else {
		// did not match
		static_assert(Traits::isCallable, "forEach(): fn cannot be applied");
	}
}

TEMPLATE_IE
template<bool WithEntity, class... Cs>
void VIEW_IE::_forEach(auto&& fn, Typelist<Cs...> wanted) {
	constexpr auto cpoolsCast = wanted.transform<_details::cpoolCast>();

	const auto cpoolsBegin = _cpools.cbegin();
	const auto cpoolsMin = cpoolsBegin + _minCpoolIdx;
	const auto cpoolsMinNext = cpoolsMin + 1;
	const auto cpoolsEnd = _cpools.cend();

	for (const auto entity : (*cpoolsMin)->_dense) {
		auto contains = [&](const auto cpool) {
			return cpool && cpool->contains(entity);
		};

		bool validEntity = Traits::hasNotNull(entity) && std::all_of(cpoolsBegin, cpoolsMin, contains) &&
			std::all_of(cpoolsMinNext, cpoolsEnd, contains);
		if constexpr (excludes.size() != 0) { // check excludes
			validEntity = validEntity && std::ranges::none_of(_cpoolsExcl, contains);
		}

		if (validEntity) {
			// find and pass all components
			if constexpr (WithEntity) {
				fn(entity,
				   reinterpret_cast<getType<cpoolsCast.get<wanted.find(typelist<Cs>)>()>>(
					   _cpools[includes.find(typelist<Cs>)]
				   )
					   ->get(entity)...);
			} else {
				fn(reinterpret_cast<getType<cpoolsCast.get<wanted.find(typelist<Cs>)>()>>(
					   _cpools[includes.find(typelist<Cs>)]
				)
					   ->get(entity)...);
			}
		}
	}
}

TEMPLATE_IE
auto VIEW_IE::comps() noexcept {
	constexpr auto nonFlags = _nonFlags();
	static_assert(nonFlags.size() != 0, "Cannot call comps() on flag-only views");
	if constexpr (includes.size() == 1) { // single include, return cpool
		constexpr auto cpool = nonFlags.transform<_details::cpoolCast>();

		if (_cpools.front()) {
			return std::views::zip(std::views::all(*reinterpret_cast<getType<cpool>>(_cpools.front())));
		} else {
			static std::remove_pointer_t<getType<cpool>> dummy;
			return std::views::zip(std::views::all(dummy));
		}
	} else {
		return _comps(nonFlags);
	}
}

TEMPLATE_IE
template<class... Cs>
auto VIEW_IE::_comps(Typelist<Cs...> wanted) noexcept {
	constexpr auto cpoolsCast = wanted.transform<_details::cpoolCast>();

	return std::views::all(*this) | std::views::transform([_cpools = _cpools](const Entity entity) {
			   return std::tie(
				   reinterpret_cast<getType<cpoolsCast.get<typelist<Cs...>.find(typelist<Cs>)>()>>(
					   _cpools[includes.find(typelist<Cs>)]
				   )
					   ->get(entity)...
			   );
		   });
}

TEMPLATE_IE
auto VIEW_IE::entityComps() noexcept {
	constexpr auto nonFlags = _nonFlags();
	static_assert(nonFlags.size() != 0, "Cannot call entityComps() on flag-only views");
	if constexpr (includes.size() == 1) {
		constexpr auto cpool = nonFlags.transform<_details::cpoolCast>();

		if (_cpools.front()) {
			return std::views::zip(
				std::views::all(*this),
				std::views::all(*reinterpret_cast<getType<cpool>>(_cpools.front()))
			);
		} else {
			static std::remove_pointer_t<getType<cpool>> dummy;
			return std::views::zip(std::views::all(*this), std::views::all(dummy));
		}
	} else {
		return _entityComps(nonFlags);
	}
}

TEMPLATE_IE
template<class... Cs>
auto VIEW_IE::_entityComps(Typelist<Cs...> wanted) noexcept {
	constexpr auto cpoolsCast = wanted.transform<_details::cpoolCast>();

	return std::views::all(*this) | std::views::transform([_cpools = _cpools](const Entity entity) {
			   return std::tuple_cat(
				   std::tuple(entity),
				   std::tie(
					   reinterpret_cast<getType<cpoolsCast.get<typelist<Cs...>.find(typelist<Cs>)>()>>(
						   _cpools[includes.find(typelist<Cs>)]
					   )
						   ->get(entity)...
				   )
			   );
		   });
}

TEMPLATE_IE
auto VIEW_IE::comps(const Entity entity) noexcept {
	return _comps(entity, _nonFlags());
}

TEMPLATE_IE
template<class... Cs>
auto VIEW_IE::_comps(const Entity entity, Typelist<Cs...> wanted) noexcept {
	constexpr auto cpoolsCast = wanted.transform<_details::cpoolCast>();

	return std::tie(
		reinterpret_cast<getType<cpoolsCast.get<wanted.find(typelist<Cs>)>()>>(_cpools[includes.find(typelist<Cs>)])
			->get(entity)...
	);
}

TEMPLATE_IE
auto VIEW_IE::compsOpt(const Entity entity) noexcept {
	return _comps(entity, _nonFlags());
}

TEMPLATE_IE
template<class... Cs>
auto VIEW_IE::_compsOpt(const Entity entity, Typelist<Cs...> wanted) noexcept
	-> std::optional<decltype(_comps(entity, wanted))> {
	if (!contains(entity)) {
		return std::nullopt;
	}

	constexpr auto cpoolsCast = wanted.transform<_details::cpoolCast>();

	return std::tie(
		reinterpret_cast<getType<cpoolsCast.get<wanted.find(typelist<Cs>)>()>>(_cpools[includes.find(typelist<Cs>)])
			->get(entity)...
	);
}

TEMPLATE_IE
bool VIEW_IE::contains(const Entity entity) const noexcept {
	auto contains = [&](const auto cpool) {
		return cpool && cpool->contains(entity);
	};

	bool contained = Traits::hasNotNull(entity) && std::ranges::all_of(_cpools, contains);
	if constexpr (excludes.size() != 0) {
		contained = contained && std::none_of(_cpoolsExcl, contains);
	}

	return contained;
}

TEMPLATE_IE
u32 VIEW_IE::minCPool() const noexcept {
	return _minCpoolIdx;
}

} // namespace arch::ecs

#undef VIEW_IE
#undef TEMPLATE_IE
