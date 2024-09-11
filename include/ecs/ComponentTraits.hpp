#include "ComponentTraits.h"

#define TEMPLATE template<class C, class E>
#define TRAITS ComponentTraits<C, E>

namespace arch::ecs::_details {

TEMPLATE
char* TRAITS::newPage() noexcept {
	return (char*)operator new[](pageSize * sizeof(C), (std::align_val_t)alignof(C[pageSize]), std::nothrow);
}

TEMPLATE
void TRAITS::deletePage(C** pages, size_t pageNum, const std::vector<E>& dense) noexcept {
	using ETraits = EntityTraits<E>;

	Type*& componentPage = pages[pageNum];
	if (not componentPage) {
		return;
	}

	const size_t offset = pageNum * pageSize;
	for (size_t i = 0; i != pageSize and offset + i < dense.size(); ++i) {
		auto&& entity = dense[offset + i];
		if (not ETraits::Version::hasNull(entity)) {
			destroyAt(componentPage + i);
		}
	}

	operator delete[](componentPage, (std::align_val_t)alignof(C[pageSize]), std::nothrow);
	componentPage = nullptr;
}

TEMPLATE
void TRAITS::destroyAt(Type* component) noexcept {
	component->~Type();
}

TEMPLATE
template<class... Args>
TRAITS::Type* TRAITS::createAt(Type* component, Args&&... args) noexcept {
	return new (component) Type(std::forward<Args>(args)...);
}

} // namespace arch::ecs::_details

#undef TEMPLATE
#undef TRAITS
