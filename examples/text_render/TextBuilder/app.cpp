#include "app.h"

#include <chrono>
#include <execution>
#include <print>

#include <archimedes/Camera.h>
#include <archimedes/Ecs.h>
#include <archimedes/Gfx.h>
#include <archimedes/Scene.h>
#include <archimedes/Text.h>
#include <archimedes/gfx/Texture.h>
#include <archimedes/scene/Components.h>
#include <unilbrk.h>
#include <unistr.h>

using namespace arch;

void TestApp::init() {
	auto scene = createRef<Scene>();
	scene::SceneManager::get()->changeScene(scene);

	auto&& fontDB = font::FontDB::get();
	auto& arial = fontDB["Arial"]->regular()->assure();

	constexpr std::u32string_view text = U"foo |\u{a0}BAR\n\n  XD\vfoo_Bar";

	auto& camera = scene->domain().global<Camera>();




	auto e = scene->newEntity();
	auto&& t = e.addComponent<scene::components::TransformComponent>({
		{ -500.f, 200, 0.f },
		{ 0.f, 0.f, 0.f, 1.f },
		{ 70.f, 70.f, 0.f }
	});

	text::TextBuilder textBuilder;
	auto&& c = e.addComponent<scene::components::MeshComponent>(textBuilder.face(arial).text(text).build(camera));
}

void TestApp::update() {
	auto& scene = *scene::SceneManager::get()->currentScene();
	if (input::Keyboard::esc.pressed()) {
		throw 0;
	}

	if (input::Mouse::scroll.y() > 0) {
		scene.domain().global<Camera>().zoomIn(1.1);
	} else if (input::Mouse::scroll.y() < 0) {
		scene.domain().global<Camera>().zoomOut(1.1);
	}

	if (input::Keyboard::arrowUp.down()) {
		auto&& [textTransform] = scene.domain().view<scene::components::TransformComponent>().comps().front();
		textTransform.scale.x += 1;
		textTransform.scale.y += 1;
	}
	if (input::Keyboard::arrowDown.down()) {
		auto&& [textTransform] = scene.domain().view<scene::components::TransformComponent>().comps().front();
		textTransform.scale.x -= 1;
		textTransform.scale.y -= 1;
	}
}
