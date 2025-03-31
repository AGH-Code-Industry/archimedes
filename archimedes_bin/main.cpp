#include "examples/Examples.h"
#include <Engine.h>
#include <Logger.h>

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Unique<arch::Application> myApp = arch::createUnique<TextRenderTestApp>();

	arch::EngineConfig config{ .windowWidth = 1'200,
							   .windowHeight = 600,
							   .windowTitle = "Archimedes Test",
							   .backgroundColor = arch::Color(.0f, .0f, .0f, 1.f),
							   .renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK };

	arch::Engine engine{ config, myApp };
	engine.start();
}
