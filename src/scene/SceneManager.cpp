#include "scene/SceneManager.h"

#include "Ecs.h"
#include "scene/Components.h"
#include "text/TextComponent.h"

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
		auto&& domain = _currentScene->domain();

		// static const auto ortho = glm::ortho(0.f, 640.f, 0.f, 400.f);

		struct RenderInfo {
			const asset::mesh::Mesh* mesh;
			const Ref<gfx::pipeline::Pipeline>* pipeline;
			const components::TransformComponent* transform;
		};

		std::vector<RenderInfo> renderInfo;

		for (auto [entity, transform, mesh] :
			 domain.view<components::TransformComponent, components::MeshComponent>().all()) {
			renderInfo.emplace_back(mesh.mesh.get(), &mesh.pipeline, &transform);
		}
		for (auto [entity, transform, text] :
			 domain.view<components::TransformComponent, text::TextComponent>().all()) {
			renderInfo.emplace_back(text.mesh().get(), &text.pipeline(), &transform);
		}

		std::ranges::sort(renderInfo, [](const RenderInfo& lhs, const RenderInfo& rhs) {
			return lhs.transform->position.z > rhs.transform->position.z;
		});

		for (auto&& [mesh, pipeline, transform] : renderInfo) {
			renderer->usePipeline(*pipeline);
			renderer->draw(
				mesh->getVertexBuffer(),
				mesh->getIndexBuffer(),
				/*ortho * */ transform->getTransformMatrix()
			);
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
