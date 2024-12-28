#pragma once

#include <unordered_set>

#include <Ecs.h>
#include <Hier.h>

namespace arch::scene {

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
	void removeEntity(const ecs::Entity entity) noexcept;

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

	bool addTag(const ecs::Entity entity, std::string_view tag) noexcept;
	bool removeTag(const ecs::Entity entity, std::string_view tag) noexcept;
	bool hasTag(const ecs::Entity entity, std::string_view tag) const noexcept;

	OptRef<const std::unordered_set<std::string_view>> tagsOf(const ecs::Entity entity) const noexcept;

private:

	void _untagEntity(const ecs::Entity entity) noexcept;

	ecs::Domain _domain;
	Node* _rootNode;
};

} // namespace arch::scene
