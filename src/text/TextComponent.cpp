#include <font/Face.h>
#include <font/Font.h>
#include <font/FontDB.h>
#include <font/FontNotFoundException.h>
#include <text/TextComponent.h>

namespace arch::text {

TextComponent::TextComponent(std::u32string text, std::string_view familyName, std::string_view styleName):
	TextComponent(std::move(text), {}, familyName, styleName) {}

TextComponent::TextComponent(
	std::u32string text,
	std::vector<Ref<gfx::buffer::Buffer>> buffers,
	std::string_view familyName,
	std::string_view styleName
):
	_text{ std::move(text) } {
	auto fontOpt = font::FontDB::get()[familyName];
	if (!fontOpt) {
		throw font::FontNotFoundException(std::format("Font '{}' not found! Try FontDB::exists", familyName));
	}

	auto faceOpt = fontOpt.get()[styleName];
	if (!fontOpt) {
		throw font::FontNotFoundException(
			std::format("Style '{}' not found in font '{}'! Try Font::hasStyle", styleName, familyName)
		);
	}

	_face = &*faceOpt;

	_compute(std::move(buffers));
}

TextComponent::TextComponent(std::u32string text, font::Face& face) noexcept:
	TextComponent(std::move(text), {}, face) {}

TextComponent::TextComponent(
	std::u32string text,
	std::vector<Ref<gfx::buffer::Buffer>> buffers,
	font::Face& face
) noexcept:
	_text{ std::move(text) },
	_face{ &face } {
	_compute(std::move(buffers));
}

void TextComponent::swap(TextComponent& other) noexcept {
	if (this == &other) {
		return;
	}

	std::swap(_text, other._text);
	std::swap(_face, other._face);
	std::swap(_topLeft, other._topLeft);
	std::swap(_bottomRight, other._bottomRight);
	std::swap(_pipeline, other._pipeline);
	std::swap(_mesh, other._mesh);
}

float3 TextComponent::topLeft(const float3 scale) const noexcept {
	return _topLeft * scale;
}

float3 TextComponent::bottomRight(const float3 scale) const noexcept {
	return _bottomRight * scale;
}

const Ref<gfx::pipeline::Pipeline>& TextComponent::pipeline() const noexcept {
	return _pipeline;
}

const Ref<asset::mesh::Mesh>& TextComponent::mesh() const noexcept {
	return _mesh;
}

void TextComponent::_compute(std::vector<Ref<gfx::buffer::Buffer>> buffers) noexcept {
	_face->assure();

	struct Vertex {
		float3 position{};
		float2 texCoords{};
	};

	std::vector<Vertex> vertices;
	vertices.reserve(4 * _text.length());
	std::vector<u32> indices;
	indices.reserve(6 * _text.length());

	u32 indicesIdx = 0;

	auto localMin = float3(std::numeric_limits<float>::max());
	auto localMax = float3(std::numeric_limits<float>::lowest());

	float3 pos{};

	for (u32 i = 0; i != _text.length(); ++i) {
		auto kerning = i != 0 ? _face->kerning(_text[i - 1], _text[i]) : 0.f;

		auto&& gd = _face->glyphData(_text[i]).getOr(_face->placeholder());
		if (gd.advanceOnly()) {
			pos.x += gd.advance + kerning;
			continue;
		}

		pos.x += kerning;

		auto tempPos = pos + float3{ gd.planeBounds.left, -gd.planeBounds.top, 0 };

		vertices.emplace_back(-tempPos, float2{ gd.uv.first.x, gd.uv.first.y });
		vertices.emplace_back(
			float3{ -gd.width / _face->resolution(), 0, 0 } - tempPos,
			float2{ gd.uv.second.x, gd.uv.first.y }
		);
		vertices.emplace_back(
			float3{ 0, gd.height / _face->resolution(), 0 } - tempPos,
			float2{ gd.uv.first.x, gd.uv.second.y }
		);
		vertices.emplace_back(
			float3{ -gd.width / _face->resolution(), gd.height / _face->resolution(), 0 } - tempPos,
			float2{ gd.uv.second.x, gd.uv.second.y }
		);

		for (const auto& v : vertices | std::views::reverse | std::views::take(4)) {
			localMin.x = std::min(localMin.x, v.position.x);
			localMin.y = std::min(localMin.y, v.position.y);

			localMax.x = std::max(localMax.x, v.position.x);
			localMax.y = std::max(localMax.y, v.position.y);
		}

		const auto indicesDelta = 4 * indicesIdx;
		indices.push_back(2 + indicesDelta);
		indices.push_back(0 + indicesDelta);
		indices.push_back(1 + indicesDelta);
		indices.push_back(3 + indicesDelta);
		indices.push_back(2 + indicesDelta);
		indices.push_back(1 + indicesDelta);
		++indicesIdx;

		pos.x += gd.advance;
	}

	_topLeft = float3{ localMax.x, localMin.y, 0 };
	_bottomRight = float3{ localMin.x, localMax.y, 0 };

	_mesh = asset::mesh::Mesh::create<Vertex>(vertices, indices);
	_pipeline =
		gfx::Renderer::getCurrent()->getPipelineManager()->create({ .vertexShaderPath = "shaders/vertex_default.glsl",
																	.fragmentShaderPath =
																		"shaders/text/fragment_atlas.glsl",
																	.textures = { _face->atlasTexture() },
																	.buffers = std::move(buffers) });
}

} // namespace arch::text
