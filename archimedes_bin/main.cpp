#include <Engine.h>
#include <Logger.h>

struct MyApp: arch::Application {
	void init() override { arch::Logger::info("Initializing user app!"); }
};

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Ref<MyApp> myApp = arch::createRef<MyApp>();

	arch::EngineConfig config{
		.windowWidth = 600,
		.windowHeight = 480,
		.windowTitle = "Archimedes Test",
		.backgroundColor = arch::Color(0, 0, 0, 0),
		.renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK
	};

	arch::Engine engine{ config, myApp };
	engine.start();
}
