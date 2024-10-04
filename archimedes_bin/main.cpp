#include <Engine.h>
#include <Logger.h>

struct MyApp: arch::Application {
	void init() override { arch::Logger::info("Initializing user app!"); }
};

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::EngineConfig config{ 600, 480, "Archimedes Test", glm::vec4(0, 0, 0, 0) };

	arch::Ref<MyApp> myApp = arch::createRef<MyApp>();

	arch::Engine engine{ config, myApp };
	engine.start();
}
