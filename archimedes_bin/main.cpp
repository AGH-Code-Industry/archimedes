#include <Engine.h>
#include <Logger.h>

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Logger::trace("Hello, {}!", "World");
	arch::Logger::debug("Hello, {}!", "World");
	arch::Logger::info("Hello, {}!", "World");
	arch::Logger::warn("Hello, {}!", "World");
	arch::Logger::error("Hello, {}!", "World");
	arch::Logger::critical("Hello, {}!", "World");

	arch::Logger::log(arch::LogLevel::info, "Info log");

	arch::EngineConfig config{ .windowWidth = 600,
							   .windowHeight = 480,
							   .windowTitle = "Archimedes Test",
							   .backgroundColor = arch::Color(0, 0, 0, 0),
							   .renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK };

	arch::Engine engine{ config };
	engine.start();
}
