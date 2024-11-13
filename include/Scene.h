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

	/// @brief Sets parent node of entity node
	/// @param entity - node to set parent of
	/// @param newParent - new parent node
	/// @return whether set was successful
	bool setParent(Node& entity, Node& newParent) noexcept;
	/// @brief Sets parent node of entity
	/// @param entity - entity to set parent of
	/// @param newParent - new parent
	/// @return whether set was successful
	bool setParent(const ecs::Entity entity, const ecs::Entity newParent) noexcept;
	/// @brief Checks if entity node is a child of parent node
	/// @param entity - child node
	/// @param parent - parent node
	bool isChildOf(const Node& entity, const Node& parent) const noexcept;
	/// @brief Checks if entity is a child of parent
	/// @param entity - child entity
	/// @param parent - parent entity
	bool isChildOf(const ecs::Entity entity, const ecs::Entity parent) const noexcept;
	/// @brief Gets ancestor entity of node
	/// @param entity - node to get ancestor of
	/// @param which - steps upward
	ecs::Entity getAncestor(const Node& entity, u32 which = 1) const noexcept;
	/// @brief Gets ancestor entity of entity
	/// @param entity - entity to get ancestor of
	/// @param which - steps upward
	ecs::Entity getAncestor(const ecs::Entity entity, const u32 which = 1) const noexcept;
	/// @brief Returns ancestry relation between two nodes (depth difference)
	/// @param ancestor - potential ancestor node
	/// @param descendant - potential descendant node
	/// @return == 0 when nodes are not directly related
	/// @return != 0 when nodes are directly related
	/// @return > 0 when descendant is confirmed to be a descendant
	/// @return < 0 when descendant is actually an ancestor
	i64 ancestry(const Node& ancestor, const Node& descendant) const noexcept;
	/// @brief Returns ancestry relation between two entities (depth difference)
	/// @param ancestor - potential ancestor entity
	/// @param descendant - potential descendant entity
	/// @return == 0 when nodes are not directly related
	/// @return != 0 when nodes are directly related
	/// @return > 0 when descendant is confirmed to be a descendant
	/// @return < 0 when descendant is actually an ancestor
	i64 ancestry(const ecs::Entity ancestor, const ecs::Entity descendant) const noexcept;
	/// @brief Gets parent entity of node
	/// @param entity - entity node to get parent of
	ecs::Entity getParent(const Node& entity) const noexcept;
	/// @brief Gets parent entity of entity
	/// @param entity - entity to get parent of
	ecs::Entity getParent(const ecs::Entity entity) const noexcept;
	/// @brief Gets Nth child entity of given node
	/// @param entity - node to get child of
	/// @param index - which child to get
	ecs::Entity getChild(const Node& entity, u32 index = 0) const noexcept;
	/// @brief Gets Nth child entity of given entity
	/// @param entity - entity to get child of
	/// @param index - which child to get
	ecs::Entity getChild(const ecs::Entity entity, const u32 index = 0) const noexcept;

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

	Node& _getNode(const ecs::Entity entity) noexcept;
	const Node& _getNode(const ecs::Entity entity) const noexcept;
	auto _tryGetNode(const ecs::Entity entity) noexcept -> decltype(_domain.tryGetComponent<Node>(entity));
	auto _tryGetNode(const ecs::Entity entity) const noexcept -> decltype(_domain.tryGetComponent<Node>(entity));

	// removes node from hierarchy
	void _unparent(Node& node) noexcept;
	void _setParentUnchecked(Node& node, Node& newParent) noexcept;
};

} // namespace arch
