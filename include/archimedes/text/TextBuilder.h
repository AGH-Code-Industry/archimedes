#pragma once

#include <any>
#include <string>
#include <vector>

#include <archimedes/Camera.h>
#include <archimedes/Font.h>
#include <archimedes/Math.h>
#include <archimedes/scene/components/MeshComponent.h>

namespace arch::text {

/// @brief Data of word
struct WordData {
	/// @brief Absolute length of word
	float length{};
	/// @brief Length of word from first visible glyph to the last
	float graphLength{};
	/// @brief Original line of word
	u32 line = 0;
	/// @brief Index of first glyph of word
	u32 first{ (u32)-1 };
	/// @brief Index of past-the-last glyph of word
	u32 last{ (u32)-1 };
};

struct VertexData {
	/// @brief Vertex screen position
	float3 position{};
	/// @brief Vertex UV position
	float2 texCoords{};
};

/// @brief Return result of TextBuilder
struct WordVertexData {
	std::vector<WordData> wordData;
	std::vector<VertexData> vertices;
	std::vector<u32> indices;
	float3 topLeft{};
	float3 bottomRight{};
};

/// @brief Helper class for rendering text
class TextBuilder {
public:
	/// @brief Sets text
	/// @param str - text to set
	template<class C>
	TextBuilder& text(std::basic_string_view<C> str);
	/// @brief Sets text
	/// @param str - text to set
	template<class C>
	TextBuilder& text(const std::basic_string<C>& str);
	/// @brief Sets text
	/// @param str - text to set
	template<class C>
	TextBuilder& text(const C* str);

	/// @brief Sets face of text
	/// @param camera
	TextBuilder& face(const font::Face& face);

	/// @brief Builds MeshComponent of set text
	/// @param camera - camera to position text on
	scene::components::MeshComponent build(const Camera& camera);

private:

	TextBuilder& _setText(std::u32string&& str);

	// divides text into words and computes vertices
	WordVertexData _processText() const;

	// returns text as unicode integers
	const uint32_t* _textUC() const { return reinterpret_cast<const uint32_t*>(_text.c_str()); }

	// returns text as unicode integers
	uint32_t* _textUC() { return reinterpret_cast<uint32_t*>(_text.data()); }

	std::u32string _text = {};
	const font::Face* _face = {};
};

} // namespace arch::text

#include "TextBuilder.hpp"
