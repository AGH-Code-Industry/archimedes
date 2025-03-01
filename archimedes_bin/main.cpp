#include "Examples.h"
#include <Engine.h>
#include <Logger.h>

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Ref<arch::Application> myApp = arch::createRef<TextRenderTestApp>();

	arch::EngineConfig config{ .windowWidth = 640,
							   .windowHeight = 400,
							   .windowTitle = "Archimedes Test",
							   .backgroundColor = arch::Color(.0f, .0f, .0f, 1.f),
							   .renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK };

	arch::Engine engine{ config, myApp };
	engine.start();
}
