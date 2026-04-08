#include "InteractiveCollisionTestApp.h"
#include <archimedes/Engine.h>
#include <archimedes/Logger.h>

///@brief Spawns all possible collider shapes on the map.
///Use WASD keys to move your main rectangle, Q and E to rotate it
///When colliding with colliders, your object will change its collor to green
///When hovering your mouse on it, its color will be red

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Unique<arch::Application> myApp = arch::createUnique<physicsExample::InteractiveCollisionTestApp>();

	arch::EngineConfig config{ .windowWidth = 1'200,
							   .windowHeight = 600,
							   .windowTitle = "Archimedes Test",
							   .backgroundColor = arch::Color(.0f, .0f, .0f, 1.f),
							   .renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK };

	arch::Engine engine{ config, myApp };
	engine.start();
}
