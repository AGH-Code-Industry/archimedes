#pragma once

#include <unordered_map>

#include <asset/mesh/Mesh.h>
#include <font/Face.h>
#include <gfx/pipeline/Pipeline.h>

namespace arch::text {

/// @brief Component for text display
class TextComponent {
public:

	/// @brief Constructor
	/// @param text - text to display
	/// @param familyName - family name of font
	/// @param styleName - style name
	TextComponent(std::u32string text, std::string_view familyName, std::string_view styleName = "");
	TextComponent(
		std::u32string text,
		std::vector<Ref<gfx::buffer::Buffer>> buffers,
		std::string_view familyName,
		std::string_view styleName = ""
	);
	TextComponent(std::u32string text, font::Face& face) noexcept;
	TextComponent(std::u32string text, std::vector<Ref<gfx::buffer::Buffer>> buffers, font::Face& face) noexcept;
	TextComponent(const TextComponent&) noexcept = default;
	TextComponent(TextComponent&&) noexcept = default;

	TextComponent& operator=(const TextComponent&) noexcept = default;
	TextComponent& operator=(TextComponent&&) noexcept = default;

	void swap(TextComponent& other) noexcept;

	float3 topLeft(const float3 scale = float3(1)) const noexcept;
	float3 bottomRight(const float3 scale = float3(1)) const noexcept;

	const Ref<gfx::pipeline::Pipeline>& pipeline() const noexcept;
	const Ref<asset::mesh::Mesh>& mesh() const noexcept;

private:

	void _compute(std::vector<Ref<gfx::buffer::Buffer>> buffers) noexcept;

	std::u32string _text{};
	font::Face* _face{};
	float3 _topLeft{};
	float3 _bottomRight{};
	Ref<gfx::pipeline::Pipeline> _pipeline;
	Ref<asset::mesh::Mesh> _mesh;

	// TODO: sizes of words -> easy word wrap
	struct BBox {
		float3 topLeft;
		float3 bottomRight;
	};

	std::unordered_map<char32_t*, BBox> _wordsBBoxes;
};

} // namespace arch::text
