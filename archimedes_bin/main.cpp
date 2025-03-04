#include "examples/Examples.h"
#include <Engine.h>
#include <Logger.h>

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Ref<arch::Application> myApp = arch::createRef<ProjectSelectorApp>();

	arch::EngineConfig config{ .windowWidth = 1'280,
							   .windowHeight = 720,
							   .windowTitle = "Archimedes Test",
							   .backgroundColor = arch::Color(.0f, .0f, .0f, 1.f),
							   .renderingApi = arch::gfx::RenderingAPI::none };

	arch::Engine engine{ config, myApp };
	engine.start();
}
