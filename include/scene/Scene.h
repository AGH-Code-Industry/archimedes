#pragma once

#include <unordered_set>

#include "ReturnHandleFlag.h"
#include <Ecs.h>
#include <Hier.h>

namespace arch::scene {

class Entity;

class Scene {
public:

	/// @brief Hierarchy node class
	using Node = hier::HierarchyNode;

	/// @brief Default constructor
	Scene() noexcept;

	/// @brief Creates new entity
	/// @see arch::ecs::Domain::newEntity()
	Entity newEntity() noexcept;
	/// @brief Creates new entity
	/// @see arch::ecs::Domain::newEntity()
	ecs::Entity newEntity(ReturnHandleFlag) noexcept;
	/// @brief Kills entity
	/// @see arch::ecs::Domain::kill(const ecs::Entity)
	void removeEntity(Entity& entity) noexcept;
	/// @brief Kills entity
	/// @see arch::ecs::Domain::kill(const ecs::Entity)
	void removeEntity(const ecs::Entity entity) noexcept;

	/// @brief Returns ecs::Domain of this scene
	ecs::Domain& domain() noexcept;
	/// @brief Returns readonly ecs::Domain of this scene
	const ecs::Domain& domain() const noexcept;

	/// @brief Returns root entity
	Entity root() noexcept;
	/// @brief Returns root entity
	ecs::Entity root(ReturnHandleFlag) const noexcept;
	/// @brief Returns root node
	Node& rootNode() noexcept;
	/// @brief Returns readonly root node
	const Node& rootNode() const noexcept;

	/// @brief Adds tag to entity
	/// @param entity - entity to add tag to
	/// @param tag - tag to add
	/// @return true if added, false otherwise
	bool addTag(const ecs::Entity entity, std::string_view tag) noexcept;
	/// @brief Removes tag from entity
	/// @param entity - entity to remove tag from
	/// @param tag - tag to remove
	/// @return true if removed, false otherwise
	bool removeTag(const ecs::Entity entity, std::string_view tag) noexcept;
	/// @brief Checks if entity has tag
	/// @param entity - entity to check
	/// @param tag - tag to check
	bool hasTag(const ecs::Entity entity, std::string_view tag) const noexcept;
	/// @brief Removes all tags from entity
	/// @param entity - entity to remove tags from
	void untag(const ecs::Entity entity) noexcept;

	/// @brief Returns optional with reference to set of tags
	/// @param entity - entity to obtain tags of
	/// @return Non-empty optional if entity was ever tagged, empty optional otherwise
	OptRef<const std::unordered_set<std::string_view>> tagsOf(const ecs::Entity entity) const noexcept;

private:

	ecs::Domain _domain;
	Node* _rootNode;
};

} // namespace arch::scene
