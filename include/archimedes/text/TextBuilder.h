#pragma once

#include <any>
#include <string>
#include <vector>

#include <archimedes/Font.h>
#include <archimedes/Math.h>
#include <archimedes/utils/DefaultValue.h>

namespace arch::text {

struct WordData {
	float3 position{};
	float length{};
};

struct VertexData {
	float3 position{};
	float2 texCoords{};
};

struct WordVertexData {
	std::vector<WordData> wordData;
	std::vector<VertexData> vertices;
	std::vector<u32> indices;
	float3 topLeft{};
	float3 bottomRight{};
};

std::vector<u64> segmentateText(std::u32string_view text, float avgWordLength = 0);

struct LineBreak {
	static WordVertexData greedy(std::u32string_view text, std::span<u64> words, font::Face& face, float lineWidthEm);
	static WordVertexData KnuthPlass(std::u32string_view, std::span<u64>, font::Face&, float) = delete;
};

class TextBuilder {
public:
	enum LineBreak {
		KnuthPlass,
		greedy
	};

	template<class C>
	TextBuilder& text(std::basic_string_view<C> str);
	template<class C>
	TextBuilder& text(const std::basic_string<C>& str);
	template<class C>
	TextBuilder& text(const C* str);

	TextBuilder& face(font::Face& face);

	TextBuilder& lineBreak(LineBreak breakAlgorithm);

	TextBuilder& lineWidthEm(float em);

private:

public:
	TextBuilder& _setText(std::u32string&& str);

	WordVertexData _processText() const;

	std::u32string _text = {};
	font::Face* _face = {};

	float _lineWidthEm = {};

	bool _KnuthPlass = {};
};

} // namespace arch::text

#include "TextBuilder.hpp"
