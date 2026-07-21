import archimedes;

using arch::ecs::Domain;
using arch::utils::UUID;

#include <print>

int main() {
	Domain domain;
	auto entity = domain.newEntity();
	domain.addComponent<UUID>(entity, UUID::random());

	for (auto [entity, uuid] : domain.view<UUID>().all()) {
		std::println("{:v}: {}", entity, uuid.toString());
	}
}
