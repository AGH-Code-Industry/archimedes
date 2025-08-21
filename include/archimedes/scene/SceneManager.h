#pragma once

#include "Ref.h"
#include "Scene.h"

namespace arch::gfx {
class Renderer;
}

namespace arch::scene {

class SceneManager {
public:
	SceneManager();

	void shutdown();

	void update();
	void renderScene(const Ref<gfx::Renderer>& renderer);

	const Ref<Scene>& currentScene() const noexcept;
	void changeScene(const Ref<Scene>& scene);

private:
	Ref<Scene> _currentScene;

public:
	static Ref<SceneManager>& get();
};

} // namespace arch::scene
