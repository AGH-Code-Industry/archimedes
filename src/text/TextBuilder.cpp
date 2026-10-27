#include <archimedes/text/ConvertTo.h>
#include <archimedes/text/TextBuilder.h>

namespace arch::text {

TextBuilder& TextBuilder::_setText(std::u32string&& str) {
	_text = std::move(str);

	return *this;
}

TextBuilder& TextBuilder::face(font::Face& face) {
	_face = &face;

	return *this;
}

TextBuilder& TextBuilder::lineBreak(LineBreak breakAlgorithm) {
	_KnuthPlass = (breakAlgorithm == KnuthPlass);

	return *this;
}

TextBuilder& TextBuilder::lineWidthEm(float em) {
	_lineWidthEm = em;

	return *this;
}

WordVertexData TextBuilder::_processText() const {
	WordVertexData result;
	auto&& [wordData, vertices, indices, topLeft, bottomRight] = result;

	if (_text.empty()) {
		return result;
	}
	const auto len = _text.length();
	vertices.reserve(4 * len);
	indices.reserve(6 * len);
	// std::vector<unicode::GeneralCategory> cats;
	// cats.reserve(_text.length());

	float3 pos = {};
	u32 indicesIdx = 0;

	auto wordBegin = 0uz;
	float wordSize = 0;

	for (auto i = 0uz; i != len; ++i) {
		auto kerning = i == 0 ? 0.f : _face->kerning(_text[i - 1], _text[i]);

		auto&& glyphData = _face->glyphData(_text[i]).getOr(_face->placeholder());
		pos.x += kerning;

		if (!glyphData.advanceOnly()) {
			auto tempPos = pos + float3(glyphData.planeBounds.left, glyphData.planeBounds.top, 0);

			float positionsX[] = { 0.f, -glyphData.width / _face->resolution() };
			float positionsY[] = { 0.f, glyphData.height / _face->resolution() };
			float uvsX[] = { glyphData.uv.first.x, glyphData.uv.second.x };
			float uvsY[] = { glyphData.uv.first.y, glyphData.uv.second.y };

			for (auto&& [ys, xs] :
				 std::views::cartesian_product(std::views::zip(positionsY, uvsY), std::views::zip(positionsX, uvsX))) {
				auto&& [posX, uvX] = xs;
				auto&& [posY, uvY] = ys;

				auto&& v = vertices.emplace_back(
					float3(positionsX[i & 0b01], positionsY[i & 0b10], 0) - tempPos,
					float2(uvsX[i & 0b01], uvsY[i & 0b10])
				);
			}

			const auto indicesDelta = 4 * indicesIdx;
			for (auto&& i : { 2u, 0u, 1u, 3u, 2u, 1u }) {
				indices.push_back(i + indicesDelta);
			}
			++indicesIdx;
		}

		pos.x += glyphData.advance;
		// cats.push_back(unicode::general_category(_text[i]));

		/*if (i > 0) {
			const auto a = _text[i - 1];
			const auto b = _text[i];
			const auto aCat = cats[i - 1];
			const auto bCat = cats[i];

			auto lineBreak = lineBreakBetween(a, b, aCat, bCat);
			if (lineBreak != LineBreakClass::prohibited) {
				auto&& currentWordData = wordData.emplace_back();
				currentWordData.length = pos.x - currentWordData.position.x;

				auto word = convertTo<char>(std::u32string_view(_text).substr(wordBegin, i - wordBegin));
				std::println("{:?}", word);
				wordBegin = i;
			}
		}*/

		// _text[i + 1] is invalid
		// but is_word_boundary checks for it
		/*if (unicode::is_word_boundary(_text.data(), len, i + 1)) {
			auto&& currentWordData = wordData.back();
			currentWordData.length = pos.x - currentWordData.position.x;
			std::println("{:?}", convertTo<char>(std::u32string_view(_text).substr(wordBegin, i + 1 - wordBegin)));
			wordBegin = i + 1;
		}*/
	}

	if (wordBegin != _text.length()) {
		auto&& currentWordData = wordData.emplace_back();
		currentWordData.length = pos.x - currentWordData.position.x;

		auto word = convertTo<char>(std::u32string_view(_text).substr(wordBegin));
		std::println("{:?}", word);
	}

	return result;
}

} // namespace arch::text

