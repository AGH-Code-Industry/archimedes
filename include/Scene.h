#pragma once

#include <Ecs.h>
#include <Hier.h>

namespace arch {

class Scene {
public:

	/// @brief Hierarchy node class
	using Node = hier::HierarchyNode;

	/// @brief Default constructor
	Scene() noexcept;

	/// @brief Creates new entity
	/// @see arch::ecs::Domain::newEntity()
	ecs::Entity newEntity() noexcept;
	/// @brief Kills entity
	/// @see arch::ecs::Domain::kill(const ecs::Entity)
	void killEntity(const ecs::Entity entity) noexcept;

	/// @brief Returns ecs::Domain of this scene
	ecs::Domain& domain() noexcept;
	/// @brief Returns readonly ecs::Domain of this scene
	const ecs::Domain& domain() const noexcept;

	/// @brief Returns root entity
	ecs::Entity root() const noexcept;
	/// @brief Returns root node
	Node& rootNode() noexcept;
	/// @brief Returns readonly root node
	const Node& rootNode() const noexcept;

private:

	ecs::Domain _domain;
	Node* _rootNode;
};

} // namespace arch
