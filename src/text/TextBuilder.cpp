#include <memory>

#include <archimedes/Logger.h>
#include <archimedes/asset/mesh/Mesh.h>
#include <archimedes/text/ConvertTo.h>
#include <archimedes/text/TextBuilder.h>
#include <unilbrk.h>

namespace arch::text {

TextBuilder& TextBuilder::_setText(std::u32string&& str) {
	_text = std::move(str);

	return *this;
}

TextBuilder& TextBuilder::face(const font::Face& face) {
	_face = &face;

	return *this;
}

WordVertexData TextBuilder::_processText() const {
	WordVertexData result;
	auto&& [wordData, vertices, indices, topLeft, bottomRight] = result;

	if (_text.empty()) {
		log::error("TextBuilder: Empty text");
		return result;
	} else if (!_face) {
		log::error("TextBuilder: Face not set");
		return result;
	}

	const auto len = _text.length();
	vertices.reserve(4 * len);
	indices.reserve(6 * len);

	auto linebreaks = std::vector<char>(len, 0);
	u32_possible_linebreaks(_textUC(), len, "UTF-32", linebreaks.data());

	float3 pos = {};
	float posXGraph = 0;

	u32 indicesIdx = 0;
	float wordBegin = 0;

	u32 line = 0;
	bool inWord = false;

	auto startWord = [&](u32 startIdx) {
		inWord = true;
		wordBegin = pos.x;
		posXGraph = pos.x;
		wordData.emplace_back(0.f, 0.f, line, startIdx);
	};
	auto endWord = [&](bool newLine, u32 endIdx) {
		if (inWord) {
			auto& currentWordData = wordData.back();
			currentWordData.length = pos.x - wordBegin;
			currentWordData.graphLength = posXGraph - wordBegin;
			currentWordData.last = endIdx;
			inWord = false;
		}
		if (newLine) {
			++line;
			pos.x = 0;
			pos.y -= _face->metrics().lineHeight; // Y is flipped
		}
	};

	startWord(0u);

	for (auto i = 0uz; i != len; ++i) {
		const auto linebreak = linebreaks[i];
		if (linebreak == UC_BREAK_MANDATORY || linebreak == UC_BREAK_CR_BEFORE_LF) {
			endWord(true, i);

			if (linebreak == UC_BREAK_CR_BEFORE_LF) {
				++i;
			}
		} else {
			auto& currentWordData = wordData.back();
			if (linebreak == UC_BREAK_POSSIBLE) {
				endWord(false, i);
			}
			startWord(i);

			auto kerning = i == 0 ? 0.f : _face->kerning(_text[i - 1], _text[i]);

			auto&& glyphData = _face->glyphData(_text[i]).getOr(_face->placeholder());
			pos.x += kerning;

			if (!glyphData.advanceOnly()) {
				auto basePos = -(pos + float3(glyphData.planeBounds.left, glyphData.planeBounds.top, 0));

				float dX[] = { 0.f, -glyphData.width / _face->resolution() };
				float dY[] = { 0.f, glyphData.height / _face->resolution() };
				float uvX[] = { glyphData.uv.first.x, glyphData.uv.second.x };
				float uvY[] = { glyphData.uv.first.y, glyphData.uv.second.y };

				vertices.emplace_back(float3(dX[0], dY[0], 0) + basePos, float2(uvX[0], uvY[0]));
				vertices.emplace_back(float3(dX[1], dY[0], 0) + basePos, float2(uvX[1], uvY[0]));
				vertices.emplace_back(float3(dX[0], dY[1], 0) + basePos, float2(uvX[0], uvY[1]));
				vertices.emplace_back(float3(dX[1], dY[1], 0) + basePos, float2(uvX[1], uvY[1]));

				const auto indicesDelta = 4 * indicesIdx;
				for (auto&& i : { 2u, 0u, 1u, 3u, 2u, 1u }) {
					indices.push_back(i + indicesDelta);
				}

				++indicesIdx;
				pos.x += glyphData.advance;
				posXGraph = pos.x;
			} else {
				pos.x += glyphData.advance;
			}
		}
	}

	endWord(false, (u32)-1);

	return result;
}

scene::components::MeshComponent TextBuilder::build(const Camera& camera) {
	scene::components::MeshComponent result;

	auto data = _processText();

	result.mesh = asset::mesh::Mesh::create<text::VertexData>(data.vertices, data.indices);
	result.pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
		{ .vertexShaderPath = "shaders/vertex_default.glsl",
		  .fragmentShaderPath = "shaders/text/fragment_atlas.glsl",
		  .textures = { _face->atlasTexture() },
		  .buffers = { camera.buffer() } }
	);

	return result;
}

} // namespace arch::text

