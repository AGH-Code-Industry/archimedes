#include "Scene.h"
//
#include "Entity.h"

namespace arch::scene {

template<class... Includes, class... Excludes>
auto Scene::entitiesWith(ExcludeT<Excludes...>) noexcept {
	/*auto owningView = ;*/

	return _domain.view<Includes...>(exclude<Excludes...>) |
		std::views::transform([&](const ecs::Entity entity) { return scene::Entity(*this, entity); });

	// return std::move(std::ranges::transform_view( //
	//	std::move(owningView), //
	//	[&](const ecs::Entity entity) { return scene::Entity(*this, entity); }
	//));
}

} // namespace arch::scene
