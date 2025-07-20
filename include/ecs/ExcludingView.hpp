#include "ExcludingView.h"
//
#include <ranges>

#include "Domain.h"
#include "IsEntity.h"
#include <tUtils/Functions/CallableTraits.h>
#include <tUtils/Functions/IsApplicable.h>
#include <tUtils/LogicalTraits.h>

#define TEMPLATE_E template<class... Excludes>
#define VIEW_E View<TypeList<>, TypeList<Excludes...>>

namespace arch::ecs {

TEMPLATE_E
VIEW_E::View(Domain* domain) noexcept:
	_pool{ &domain->_entityPool },
	_excludedCpools{ (_details::CommonComponentPool*)domain->_tryGetCPool<Excludes>()... } {}

TEMPLATE_E
VIEW_E::Iterator VIEW_E::begin() const noexcept {
	return Iterator(this, 0);
}

TEMPLATE_E
VIEW_E::Iterator VIEW_E::cbegin() const noexcept {
	return begin();
}

TEMPLATE_E
VIEW_E::Iterator VIEW_E::end() const noexcept {
	return Iterator(this, _pool->_dense.size());
}

TEMPLATE_E
VIEW_E::Iterator VIEW_E::cend() const noexcept {
	return end();
}

TEMPLATE_E
template<class Fn>
void VIEW_E::forEach(Fn&& fn) noexcept {
	using Traits = tUtils::CallableTraits<Fn>;

	static_assert(tUtils::isApplicableV<Fn, std::tuple<Entity>>);

	for (auto entity : *this) {
		fn(entity);
	}
}

TEMPLATE_E
auto VIEW_E::entities() const noexcept {
	return std::views::all(*_pool);
}

TEMPLATE_E
bool VIEW_E::contains(const Entity entity) const noexcept {
	return _pool->contains(entity) && std::ranges::none_of(_excludedCpools, [entity](const auto cpool) {
			   return cpool && cpool->contains(entity);
		   });
}

} // namespace arch::ecs

#undef TEMPLATE_E
#undef VIEW_E
