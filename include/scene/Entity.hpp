#include "Entity.h"
#include "InvalidEntityException.h"
#include "Scene.h"

namespace arch::scene {

template<class C, class... Args>
Entity::GetResult<C> Entity::addComponent(Args&&... args) {
	_assertValid();
	return _scene->domain().addComponent<C>(_entity, std::forward<Args>(args)...);
}

template<class C, class... Args>
Entity::GetResultOpt<C> Entity::addComponentOpt(Args&&... args) noexcept {
	auto err = error();
	if (err == Error::none) {
		if constexpr (ComponentInfo<C>::flag) {
			return _scene->domain().addComponent<C>(_entity, std::forward<Args>(args)...);
		} else {
			return _scene->domain().addComponent<C>(_entity, std::forward<Args>(args)...);
		}
	} else {
		return Err(err);
	}
}

template<class C>
requires(!std::is_const_v<C> && !ComponentInfo<C>::flag)
Entity::GetResult<C> Entity::getComponent() {
	_assertValid();
	auto opt = _scene->domain().tryGetComponent<C>(_entity);
	return opt ?
		*opt :
		throw InvalidEntityException(std::format("Entity does not have component {}", arch::typedesc<C>().name));
}

template<class C>
Entity::GetResult<const C> Entity::getComponent() const {
	_assertValid();
	auto opt = _scene->domain().tryGetComponent<C>(_entity);
	return opt ?
		*opt :
		throw InvalidEntityException(std::format("Entity does not have component {}", arch::typedesc<C>().name));
}

template<class C>
requires(!std::is_const_v<C> && !ComponentInfo<C>::flag)
Entity::GetResultOpt<C> Entity::getComponentOpt() noexcept {
	auto err = error();
	if (err == Error::none) {
		auto opt = _scene->domain().tryGetComponent<C>(_entity);
		if (opt) {
			return *opt;
		} else {
			return Err(Error::componentNotFound);
		}
	} else {
		return Err(err);
	}
}

template<class C>
Entity::GetResultOpt<const C> Entity::getComponentOpt() const noexcept {
	auto err = error();
	if (err == Error::none) {
		auto opt = _scene->domain().tryGetComponent<const C>(_entity);
		if (opt) {
			return *opt;
		} else {
			return Err(Error::componentNotFound);
		}
	} else {
		return Err(err);
	}
}

template<class C>
bool Entity::removeComponent() {
	_assertValid();
	return _scene->domain().removeComponent<C>(_entity);
}

template<class C>
requires(ComponentInfo<C>::movable && !ComponentInfo<C>::flag)
C Entity::removeComponent(MoveFlag) {
	_assertValid();
	if (!_scene->domain().hasComponent<C>(_entity)) {
		throw InvalidEntityException(std::format("Entity does not have component {}", arch::typedesc<C>().name));
	}
	return std::move(_scene->domain().removeComponent<C>(_entity, moveFlag));
}

template<class C>
Res<bool, Entity::Error> Entity::removeComponentOpt() noexcept {
	auto err = error();
	if (err == Error::none) {
		return _scene->domain().removeComponent<C>(_entity);
	} else {
		return Err(err);
	}
}

template<class C>
requires(ComponentInfo<C>::movable && !ComponentInfo<C>::flag)
MoveRes<C, Entity::Error> Entity::removeComponentOpt(MoveFlag) noexcept {
	auto err = error();
	if (err == Error::none) {
		if (!_scene->domain.hasComponent<C>) {
			return Err(Error::componentNotFound);
		}
		return std::move(_scene->domain().removeComponent<C>(_entity, moveFlag));
	} else {
		return Err(err);
	}
}

template<class C>
bool Entity::hasComponent() const {
	_assertValid();
	return _scene->domain().hasComponent<C>(_entity);
}

template<class C>
Res<bool, Entity::Error> Entity::hasComponentOpt() const noexcept {
	auto err = error();
	if (err == Error::none) {
		return _scene->domain().hasComponent<C>(_entity);
	} else {
		return Err(err);
	}
}

} // namespace arch::scene
