#include "examples/Examples.h"
#include <Engine.h>
#include <Logger.h>

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::net::Init::init();

	// ecs::Domain domain;

	// int entityCount = 0;

	// ecs::Entity entity = domain.newEntity();
	// while (ecs::_details::EntityTraits::Id::part(entity) != 18) {
	//	entity = domain.newEntity();
	//	// Logger::debug("{:|v}, {}", entity, ecs::_details::EntityTraits::Id::part(entity));
	// }

	// domain.kill(ecs::Entity(8));
	// domain.kill(ecs::Entity(7));
	// domain.kill(ecs::Entity(14));
	// domain.kill(ecs::Entity(13));

	// entity = domain.newEntity();
	// while (ecs::_details::EntityTraits::Id::part(entity) != 22) {
	//	entity = domain.newEntity();
	// }

	// domain.kill(ecs::Entity(10));
	// domain.kill(ecs::Entity(9));
	// domain.kill(ecs::Entity(16));

	// Logger::debug("{}", domain.alive((ecs::Entity)15));
	// domain.kill(ecs::Entity(15));

	// Logger::debug("{}", domain.alive((ecs::Entity)15));
	// domain.kill(ecs::_details::EntityTraits::Ent::fromParts(14, 1));
	// domain.kill(ecs::_details::EntityTraits::Ent::fromParts(13, 1));
	// domain.kill(ecs::_details::EntityTraits::Ent::fromParts(20, 0));
	// domain.kill(ecs::_details::EntityTraits::Ent::fromParts(19, 0));

	//// Logger::debug("{}", domain.alive((ecs::Entity)15));
	// entity = domain.newEntity();

	// Logger::debug("{:|v}", entity);

	{
		arch::Ref<arch::Application> myApp = arch::createRef<ProjectSelectorApp>();

		arch::EngineConfig config{ .windowWidth = 1'280,
								   .windowHeight = 720,
								   .windowTitle = "Archimedes Test",
								   .backgroundColor = arch::Color(.0f, .0f, .0f, 1.f),
								   .renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK };

		arch::Engine engine{ config, myApp };
		engine.start();
	}

	arch::net::Init::cleanup();
}
