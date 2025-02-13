#include <ranges>

#include "Domain.h"

namespace arch::ecs {

template<class C>
ComponentPool<std::remove_const_t<C>>& Domain::_assureCPool() noexcept {
	using Comp = std::remove_const_t<C>;
	const auto type = typedesc<Comp>().wrap(); // custom RTTI gets a use

	auto found = _componentPools.find(type);
	if (found == _componentPools.end()) {
		_cpoolDestroyers[type] = _destroyCPool<Comp>;
		return *(new (_componentPools[type].storage.data()) ComponentPool<Comp>);
	}

	return *reinterpret_cast<ComponentPool<Comp>*>(found->second.storage.data());
}

template<class C>
void Domain::_destroyCPool(CPoolsT& cpools) noexcept {
	using Comp = std::remove_const_t<C>;
	reinterpret_cast<ComponentPool<Comp>*>(cpools[typedesc<Comp>().wrap()].storage.data())->~ComponentPool();
}

template<class C>
requires(!std::is_const_v<C>)
ComponentPool<C>& Domain::_getCPool() noexcept {
	return *reinterpret_cast<ComponentPool<C>*>(_componentPools.find(typedesc<C>().wrap())->second.storage.data());
}

template<class C>
const ComponentPool<std::remove_const_t<C>>& Domain::_getCPool() const noexcept {
	using Comp = std::remove_const_t<C>;
	return *reinterpret_cast<const ComponentPool<Comp>*>(
		_componentPools.find(typedesc<Comp>().wrap())->second.storage.data()
	);
}

template<class C>
requires(!std::is_const_v<C>)
ComponentPool<C>* Domain::_tryGetCPool() noexcept {
	const auto found = _componentPools.find(typedesc<C>().wrap());
	return found != _componentPools.end() ? reinterpret_cast<ComponentPool<C>*>(found->second.storage.data()) : nullptr;
}

template<class C>
const ComponentPool<std::remove_const_t<C>>* Domain::_tryGetCPool() const noexcept {
	using Comp = std::remove_const_t<C>;
	const auto found = _componentPools.find(typedesc<Comp>().wrap());
	return found != _componentPools.end() ? reinterpret_cast<const ComponentPool<Comp>*>(found->second.storage.data()) :
											nullptr;
}

template<class C, class... Args>
Domain::GetReference<std::remove_const_t<C>> Domain::addComponent(const Entity entity, Args&&... args) noexcept {
	return _assureCPool<std::remove_const_t<C>>().addComponent(entity, std::forward<Args>(args)...);
}

template<class C>
Domain::GetReference<std::remove_const_t<C>> Domain::addComponent(const Entity entity, C&& component) noexcept {
	return _assureCPool<std::remove_const_t<C>>().addComponent(entity, std::forward<C>(component));
}

template<class C>
requires(!std::is_const_v<C>)
Domain::GetReference<C> Domain::getComponent(const Entity entity) noexcept {
	return _getCPool<C>().get(entity);
}

template<class C>
Domain::ConstGetReference<std::remove_const_t<C>> Domain::getComponent(const Entity entity) const noexcept {
	return _getCPool<std::remove_const_t<C>>().get(entity);
}

template<class C>
requires(!_details::ComponentTraits<C>::flag && !std::is_const_v<C>)
OptRef<C> Domain::tryGetComponent(const Entity entity) noexcept {
	auto cpool = _tryGetCPool<C>();
	return cpool ? cpool->tryGet(entity) : std::nullopt;
}

template<class C>
requires(!_details::ComponentTraits<std::remove_const_t<C>>::flag)
OptRef<const std::remove_const_t<C>> Domain::tryGetComponent(const Entity entity) const noexcept {
	auto cpool = _tryGetCPool<std::remove_const_t<C>>();
	return cpool ? cpool->tryGet(entity) : std::nullopt;
}

template<class C>
bool Domain::removeComponent(const Entity entity) noexcept {
	return _assureCPool<std::remove_const_t<C>>().removeComponent(entity);
}

template<class C>
requires(std::movable<std::remove_const_t<C>> && !_details::ComponentTraits<std::remove_const_t<C>>::flag)
std::remove_const_t<C> Domain::removeComponent(const Entity entity, MoveFlag) noexcept {
	return _assureCPool<C>().removeComponent(entity, moveFlag);
}

template<class C>
requires(!std::is_const_v<C>)
auto Domain::components() noexcept {
	return std::views::all(_assureCPool<C>());
}

template<class C>
auto Domain::components() const noexcept {
	static const ComponentPool<C> empty;
	auto poolPtr = _tryGetCPool<C>();
	return poolPtr ? std::views::all(*poolPtr) : std::views::all(empty);
}

template<class C>
bool Domain::hasComponent(const Entity entity) const noexcept {
	auto cpool = _tryGetCPool<std::remove_const_t<C>>();
	return cpool ? cpool->contains(entity) : false;
}

template<class C>
size_t Domain::count() const noexcept {
	auto cpool = _tryGetCPool<std::remove_const_t<C>>();
	return cpool ? cpool->count() : 0;
}

template<class... Includes, class... Excludes>
auto Domain::view(ExcludeT<Excludes...>) noexcept {
	if constexpr (sizeof...(Includes) != 0) {
		/*ARCH_ASSERT(
			std::ranges::none_of(
				std::initializer_list<const _details::CommonComponentPool<E>*>{
					dynamic_cast<const _details::CommonComponentPool<E>*>(_tryGetCPool<std::remove_const_t<Includes>>()
					)... },
				[](const auto ptr) { return ptr == nullptr; }
			),
			"One of requested ComponentPools does not exist"
		);*/
		return View<false, TypeList<Includes...>, TypeList<Excludes...>>(this);
	} else {
		return View<false, TypeList<>, TypeList<Excludes...>>(this);
	}
}

template<class... Includes, class... Excludes>
auto Domain::view(ExcludeT<Excludes...>) const noexcept {
	if constexpr (sizeof...(Includes) != 0) {
		/*ARCH_ASSERT(
			std::ranges::none_of(
				std::initializer_list<const _details::CommonComponentPool<E>*>{
					dynamic_cast<const _details::CommonComponentPool<E>*>(_tryGetCPool<std::remove_const_t<Includes>>()
					)... },
				[](const auto ptr) { return ptr == nullptr; }
			),
			"One of requested ComponentPools does not exist"
		);*/
		return View<true, TypeList<Includes...>, TypeList<Excludes...>>(this);
	} else {
		return View<true, TypeList<>, TypeList<Excludes...>>(this);
	}
}

template<class... Includes, class... Excludes>
auto Domain::readonlyView(ExcludeT<Excludes...>) const noexcept {
	return view<Includes...>(exclude<Excludes...>);
}

template<class T>
T& Domain::Global::get() noexcept {
	return *reinterpret_cast<T*>(ptr);
}

template<class T>
const T& Domain::Global::get() const noexcept {
	return *reinterpret_cast<const T*>(ptr);
}

template<class T, class... Args>
T& Domain::global(Args&&... args) noexcept requires(!std::is_const_v<T>)
{
	auto&& found = _globals.find(typedesc<T>().wrap());
	if (found != _globals.end()) {
		return found->second.get<T>();
	} else {
		auto& global = _globals[typedesc<T>().wrap()];
		global.ptr = reinterpret_cast<u8*>(new T(std::forward<Args>(args)...));
		global.deleter = [](u8* ptr) {
			delete reinterpret_cast<T*>(ptr);
		};
		return global.get<T>();
	}
}

template<class T>
OptRef<T> Domain::global() const noexcept requires(std::is_const_v<T>)
{
	auto&& found = _globals.find(typedesc<std::remove_const_t<T>>().wrap());
	return found != _globals.end() ? OptRef(found.second.get<std::remove_const_t<T>>()) : std::nullopt;
}

} // namespace arch::ecs
