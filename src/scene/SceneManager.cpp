#include "scene/SceneManager.h"

#include "Ecs.h"
#include "exceptions/InitException.h"
#include "scene/Components.h"

namespace arch::scene {

SceneManager::SceneManager() {}

void SceneManager::update() {
	// TODO: update scene & all systems
}

void SceneManager::renderScene(const Ref<gfx::Renderer>& renderer) {
	if (_currentScene) {
		auto view = _currentScene->domain().view<components::TransformComponent, components::MeshComponent>();

		for (auto [entity, transform, mesh] : view.all()) {
			renderer->render(mesh.mesh, transform.getTransformMatrix());
		}
	}
}

const Ref<Scene>& SceneManager::currentScene() const noexcept {
	return _currentScene;
}

void SceneManager::changeScene(const Ref<Scene>& scene) {
	// TODO: destroy old scene

	_currentScene = scene;

	// TODO: init new scene
}

Ref<SceneManager>& SceneManager::get() {
	static Ref<SceneManager> instance = createRef<SceneManager>();
	return instance;
}

} // namespace arch::scene
