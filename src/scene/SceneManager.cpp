#include "scene/SceneManager.h"

#include "Ecs.h"
#include "scene/Components.h"

namespace arch::scene {

static Ref<SceneManager> instance{};

SceneManager::SceneManager() {}

void SceneManager::shutdown() {
	// TODO: destroy old scene
	_currentScene.reset();
	instance.reset();
}

void SceneManager::update() {
	// TODO: update scene & all systems
}

void SceneManager::renderScene(const Ref<gfx::Renderer>& renderer) {
	if (_currentScene) {
		auto view = _currentScene->domain().view<components::TransformComponent, components::MeshComponent>();

		for (auto [entity, transform, mesh] : view.all()) {
			renderer->usePipeline(mesh.pipeline);
			renderer->draw(mesh.mesh->getVertexBuffer(), mesh.mesh->getIndexBuffer(), transform.getTransformMatrix());
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
	if (!instance) {
		instance = createRef<SceneManager>();
	}
	return instance;
}

} // namespace arch::scene
