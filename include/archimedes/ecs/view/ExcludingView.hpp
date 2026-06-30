#include "ExcludingView.h"

#define TEMPLATE_E              \
	template<class... Excludes> \
	requires(sizeof...(Excludes) != 0)

#define VIEW_E View<TypeList<>, TypeList<Excludes...>>

namespace arch::ecs {

TEMPLATE_E
VIEW_E::View(Domain& domain) noexcept {
	_domain = &domain;
	_cpoolsExcl = { domain._tryGetCPool<Excludes>()... };
}

TEMPLATE_E
VIEW_E::~View() noexcept {
	this->_domain = {};
	this->_cpoolsExcl = {};
}

TEMPLATE_E
VIEW_E::Iterator VIEW_E::begin() const noexcept {
	return Iterator(*this, false);
}

TEMPLATE_E
VIEW_E::Iterator VIEW_E::end() const noexcept {
	return Iterator(*this, true);
}

TEMPLATE_E
void VIEW_E::forEach(auto&& fn) {
	for (const auto entity : _domain->entities()) {
		auto contains = [&](const auto cpool) {
			return cpool && cpool->contains(entity);
		};
		if (Traits::hasNotNull(entity) && std::ranges::none_of(_cpoolsExcl, contains)) {
			fn(entity);
		}
	}
}

TEMPLATE_E
bool VIEW_E::contains(const Entity entity) const noexcept {
	auto contains = [&](const auto cpool) {
		return cpool && cpool->contains(entity);
	};

	return Traits::hasNotNull(entity) && std::ranges::none_of(_cpoolsExcl, contains);
}

} // namespace arch::ecs
