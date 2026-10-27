#include "app.h"
#include <archimedes/Engine.h>
#include <archimedes/Ref.h>

using namespace arch;

int main() {
	Unique<Application> myApp = arch::createUnique<TestApp>();

	arch::EngineConfig config{
		.windowWidth = 1'200,
		.windowHeight = 600,
		.windowTitle = "Archimedes Test",
		.backgroundColor = arch::Color(.0f, .0f, .0f, 1.f),
		.renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK,
		.loggerConfig = { .name = "Archimedes", .level = arch::log::trace }
	};

	arch::Engine engine{ config, myApp };
	engine.start();
}
