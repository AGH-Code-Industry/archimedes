#include "RepulseTestApp.h"

#include <archimedes/Engine.h>
#include <archimedes/Logger.h>

///@brief Simple test for OBB collision
/// Two objects move to each other and they push each other in opposite directions

int main() {
	

	arch::Unique<arch::Application> myApp = arch::createUnique<physicsExample::RepulseTestApp>();

	arch::EngineConfig config{ .windowWidth = 1'200,
							   .windowHeight = 600,
							   .windowTitle = "Archimedes Test",
							   .backgroundColor = arch::Color(.0f, .0f, .0f, 1.f),
							   .renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK,.loggerConfig = { .name = "Archimedes", .level = arch::log::trace } };

	arch::Engine engine{ config, myApp };
	engine.start();
}
