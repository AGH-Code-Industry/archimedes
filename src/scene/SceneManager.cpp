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
			ecs::Entity entity;
			Ref<asset::mesh::Mesh> mesh;
			Ref<gfx::pipeline::Pipeline> pipeline;
			const components::TransformComponent* transform;
			bool destroyed;
		};

		std::vector<RenderInfo> renderInfo;

		auto meshView = domain.view<components::TransformComponent, components::MeshComponent>();
		auto textView = domain.view<components::TransformComponent, text::TextComponent>();

		for (auto&& [entity, transform, mesh] : meshView.all()) {
			renderInfo.emplace_back(entity, mesh.mesh, mesh.pipeline, &transform, mesh.destroyed);
		}
		for (auto&& [entity, transform, text] : textView.all()) {
			renderInfo.emplace_back(entity, text.mesh(), text.pipeline(), &transform, text.destroyed);
		}

		std::ranges::sort(renderInfo, [](const RenderInfo& lhs, const RenderInfo& rhs) {
			return lhs.transform->position.z > rhs.transform->position.z;
		});

		for (auto&& [e, mesh, pipeline, transform, destroyed] : renderInfo) {
			if (!domain.alive(e)) {
				Logger::critical("{:|v} is not alive", e);
			}
			if (destroyed) {
				Logger::critical("{:|v} was destroyed", e);
			}
			renderer->usePipeline(pipeline);
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
