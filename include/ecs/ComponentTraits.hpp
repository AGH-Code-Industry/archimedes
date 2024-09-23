#include "ComponentTraits.h"

#define TEMPLATE_CE template<class C, class E>
#define TRAITS_CE ComponentTraits<C, E>

namespace arch::ecs::_details {

TEMPLATE_CE
typename TRAITS_CE::ComponentT* TRAITS_CE::newPage() noexcept {
	return (ComponentT*)
	operator new[](pageSize * sizeof(ComponentT), (std::align_val_t)alignof(C[pageSize]), std::nothrow);
}

TEMPLATE_CE
void TRAITS_CE::deletePage(ComponentT** pages, size_t pageNum, const std::vector<E>& dense) noexcept {
	using ETraits = EntityTraits<E>;

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

TEMPLATE_CE
void TRAITS_CE::destroyAt(ComponentT* component) noexcept {
	component->~ComponentT();
}

TEMPLATE_CE
template<class... Args>
TRAITS_CE::ComponentT& TRAITS_CE::constructAt(ComponentT* component, Args&&... args) noexcept {
	return *new (component) ComponentT(std::forward<Args>(args)...);
}

} // namespace arch::ecs::_details

#undef TEMPLATE_CE
#undef TRAITS_CE
