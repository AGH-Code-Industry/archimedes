#include "ComponentTraits.h"

#define TEMPLATE_C template<class C>
#define TRAITS_C ComponentTraits<C>

namespace arch::ecs::_details {

TEMPLATE_C
typename TRAITS_C::ComponentT* TRAITS_C::newPage() noexcept {
	return (ComponentT*)
	operator new[](pageSize * sizeof(ComponentT), (std::align_val_t)alignof(C[pageSize]), std::nothrow);
}

TEMPLATE_C
void TRAITS_C::deletePage(ComponentT** pages, size_t pageNum, const std::vector<Entity>& dense) noexcept {
	using ETraits = EntityTraits;

	ComponentT*& componentPage = pages[pageNum];
	if (!componentPage) {
		return;
	}

	const size_t offset = pageNum * pageSize;
	for (size_t i = 0; i != pageSize && offset + i < dense.size(); ++i) {
		auto&& entity = dense[offset + i];
		if (!ETraits::Version::hasNull(entity)) {
			destroyAt(componentPage + i);
		}
	}

	operator delete[](componentPage, (std::align_val_t)alignof(C[pageSize]), std::nothrow);
	componentPage = nullptr;
}

TEMPLATE_C
void TRAITS_C::destroyAt(ComponentT* component) noexcept {
	component->~ComponentT();
}

TEMPLATE_C
template<class... Args>
TRAITS_C::ComponentT& TRAITS_C::constructAt(ComponentT* component, Args&&... args) noexcept {
	return *new (component) ComponentT(std::forward<Args>(args)...);
}

TEMPLATE_C
template<class... Args>
TRAITS_C::ComponentT& TRAITS_C::constructAt(ComponentT* component, ComponentT&& toMove) noexcept {
	return *new (component) ComponentT(std::move(toMove));
}

} // namespace arch::ecs::_details

#undef TEMPLATE_C
#undef TRAITS_C
