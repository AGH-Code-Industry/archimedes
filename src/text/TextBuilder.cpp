#include <archimedes/text/TextBuilder.h>
#include <unicodelib.h>

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

// Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et
// dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea
// commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla
// pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est
// laborum.

std::vector<WordData> TextBuilder::_processText() const {
	return _processTextGreedy();
}

std::vector<WordData> TextBuilder::_processTextGreedy() const {
	std::vector<WordData> result;
	if (_text.empty()) {
		return result;
	}

	const auto len = _text.length();
	// i <= len is planned
	auto prev = 0uz;
	for (auto i = 1uz; i <= len; ++i) {
		if (unicode::is_word_boundary(_text.data(), len, i)) {
		}
	}

	return result;
}

std::vector<u64> segmentateText(std::u32string_view text, float avgWordLength) {
	std::vector<u64> result;
	if (text.empty()) {
		return result;
	}

	const auto len = text.length();
	if (avgWordLength != 0) {
		result.reserve(size_t(len / avgWordLength));
	}
	result.push_back(0);

	auto prev = 0uz;
	for (auto i = 1uz; i < len; ++i) {
		if (unicode::is_word_boundary(text.data(), len, i)) {
			result.push_back(i);
			prev = i;
		}
	}

	return result;
}

std::vector<WordData> LineBreak::greedy(
	std::u32string_view text,
	std::span<u64> words,
	font::Face& face,
	float lineWidthEm
) {
	std::vector<WordData> result;
	result.reserve(words.size());

	float currentLength = 0;
	for (auto&& [begin, end] : words | std::views::pairwise) {
	}
}

} // namespace arch::text

