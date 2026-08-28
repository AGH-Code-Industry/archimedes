#pragma once

#include <any>
#include <vector>

#include <archimedes/Font.h>
#include <archimedes/Math.h>
#include <archimedes/utils/DefaultValue.h>

namespace arch::text {

struct WordData {
	float3 position{};
	float length{};
};

std::vector<u64> segmentateText(std::u32string_view text, float avgWordLength = 0);

struct LineBreak {
	static std::vector<WordData> greedy(
		std::u32string_view text,
		std::span<u64> words,
		font::Face& face,
		float lineWidthEm
	);
	static std::vector<WordData> KnuthPlass(std::u32string_view, std::span<u64>, font::Face&, float) = delete;
};

class TextBuilder {
public:
	enum Align {
		left,
		center,
		right,
		justifiy
	};

	enum LineBreak {
		KnuthPlass,
		greedy
	};

	template<class C>
	TextBuilder& text(std::basic_string_view<C> str);
	template<class C>
	TextBuilder& text(const std::basic_string<C>& str);

	TextBuilder& face(font::Face& face);

	TextBuilder& lineBreak(LineBreak breakAlgorithm);

	TextBuilder& lineWidthEm(float em);

	TextBuilder& lineSpacingBefore(float spacingBefore);
	TextBuilder& lineSpacingBefore(utils::DefaultValue);

	TextBuilder& lineSpacingAfter(float spacingAfter);
	TextBuilder& lineSpacingAfter(utils::DefaultValue);

	TextBuilder& lineSpacing(float spacing);
	TextBuilder& lineSpacing(utils::DefaultValue);

private:
	TextBuilder& _setText(std::u32string&& str);

	std::vector<WordData> _processText() const;
	std::vector<WordData> _processTextGreedy() const;
	std::vector<WordData> _processTextKnuthPlass() const = delete;

	std::u32string _text = {};
	font::Face* _face = {};

	float _lineWidthEm = {};

	float _spacingBefore = {};
	float _spacingAfter = {};
	float _lineSpacing = {};

	bool _KnuthPlass = {};
};

} // namespace arch::text

#include "TextBuilder.hpp"
