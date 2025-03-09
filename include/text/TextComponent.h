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
	/// @param styleName - style name (optional, default value "")
	/// @throws FontException if font was not found
	TextComponent(std::u32string text, std::string_view familyName, std::string_view styleName = "");
	/// @brief Constructor
	/// @param text - text to display
	/// @param buffers - buffers to pass to pipeline
	/// @param familyName - family name of font
	/// @param styleName - style name (optional, default value "")
	/// @throws FontException if font was not found
	TextComponent(
		std::u32string text,
		std::vector<Ref<gfx::buffer::Buffer>> buffers,
		std::string_view familyName,
		std::string_view styleName = ""
	);
	/// @brief Constructor
	/// @param text - text to display
	/// @param face - face of text to display
	TextComponent(std::u32string text, font::Face& face) noexcept;
	/// @brief Constructor
	/// @param text - text to display
	/// @param buffers - buffers to pass to pipeline
	/// @param face - face of text to display
	TextComponent(std::u32string text, std::vector<Ref<gfx::buffer::Buffer>> buffers, font::Face& face) noexcept;
	/// @brief Copy constructor
	TextComponent(const TextComponent&) noexcept = default;
	/// @brief Move constructor
	TextComponent(TextComponent&& other) noexcept;

	~TextComponent() { destroyed = true; }

	/// @brief Copy-assignment operator
	TextComponent& operator=(const TextComponent&) noexcept = default;
	/// @brief Move-assignment operator
	TextComponent& operator=(TextComponent&& other) noexcept;

	/// @brief Swap function
	void swap(TextComponent& other) noexcept;

	/// @brief Returns untransformed top-left corner of text
	float3 topLeft() const noexcept;
	/// @brief Returns top-left corner of text
	/// @param transformMat - matrix to transform topLeft
	float3 topLeft(const Mat4x4& transformMatrix) const noexcept;
	/// @brief Returns untransformed adjusted top-left corner of text
	/// @details Adjusted means that result's .x == baseline.x
	float3 topLeftAdjusted() const noexcept;
	/// @brief Returns adjusted top-left corner of text
	/// @details Adjusted means that result's .x == baseline.x
	/// @param transformMat - matrix to transform topLeft
	float3 topLeftAdjusted(const Mat4x4& transformMatrix) const noexcept;
	/// @brief Returns untransformed bottom-right corner of text
	float3 bottomRight() const noexcept;
	/// @brief Returns bottom-right corner of text
	/// @param transformMat - matrix to transform topLeft
	float3 bottomRight(const Mat4x4& transformMatrix) const noexcept;

	/// @brief Returns untransformed top-right corner of text
	float3 topRight() const noexcept;
	/// @brief Returns top-right corner of text
	/// @param transformMat - matrix to transform toRight
	float3 topRight(const Mat4x4& transformMatrix) const noexcept;
	/// @brief Returns untransformed bottom-right corner of text
	float3 bottomLeft() const noexcept;
	/// @brief Returns bottom-right corner of text
	/// @param transformMat - matrix to transform bottomLeft
	float3 bottomLeft(const Mat4x4& transformMatrix) const noexcept;
	/// @brief Returns untransformed adjusted bottom-left corner of text
	/// @details Adjusted means that result's .x == baseline.x
	float3 bottomLeftAdjusted() const noexcept;
	/// @brief Returns adjusted bottom-left corner of text
	/// @details Adjusted means that result's .x == baseline.x
	/// @param transformMat - matrix to transform bottomLeft
	float3 bottomLeftAdjusted(const Mat4x4& transformMatrix) const noexcept;

	const std::u32string& string() const noexcept { return _text; }

	/// @brief Returns Ref to pipeline of text
	const Ref<gfx::pipeline::Pipeline>& pipeline() const noexcept;
	/// @brief Returns Ref to mesh of text
	const Ref<asset::mesh::Mesh>& mesh() const noexcept;

	bool destroyed = false;

private:

	// computes everything and creates mesh and pipeline
	void _compute(std::vector<Ref<gfx::buffer::Buffer>> buffers) noexcept;

	std::u32string _text{};
	font::Face* _face{};
	float3 _topLeft{};
	float3 _bottomRight{};
	Ref<gfx::pipeline::Pipeline> _pipeline;
	Ref<asset::mesh::Mesh> _mesh;

	// TODO: sizes of words -> easy word wrap
	/*struct BBox {
		float3 topLeft;
		float3 bottomRight;
	};

	std::unordered_map<char32_t*, BBox> _wordsBBoxes;*/
};

} // namespace arch::text
