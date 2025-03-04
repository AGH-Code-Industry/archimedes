#include <format>
#include <iostream>

#include <scene/components/MeshComponent.h>
#include <scene/components/TransformComponent.h>
#include <text/TextComponent.h>

namespace arch::text {

TextComponent::TextComponent(ecs::Domain& domain, const ecs::Entity entity) noexcept:
	_domain{ &domain },
	_entity{ entity } {
	_domain
		->addComponent<scene::components::TransformComponent>(_entity, float3{}, Quat{ 0, 0, 0, 1 }, float3{ 1, 1, 0 });
}

TextComponent& TextComponent::operator=(font::Face& face) noexcept {
	return setFont(face);
}

TextComponent& TextComponent::setFont(std::string_view familyName, std::string_view styleName) {
	auto&& fontDB = font::FontDB::get();

	auto fontOpt = fontDB[familyName];
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
	_face->assure();

	return *this;
}

TextComponent& TextComponent::setFont(font::Face& face) {
	_face = &face;
	_face->assure();
	return *this;
}

TextComponent& TextComponent::setPosition(float2 pos) {
	auto&& t = _domain->addComponent<scene::components::TransformComponent>(_entity);
	// auto posCopy = t.position;
	t.position = float3{ pos.x, pos.y, 0 };
	// auto delta = float2{ t.position.x - posCopy.x, t.position.y - posCopy.y };
	// this->_topLeft += delta;
	// this->_bottomRight += delta;
	return *this;
}

TextComponent& TextComponent::setRotation(float radians) {
	_domain->addComponent<scene::components::TransformComponent>(_entity).rotation =
		glm::angleAxis(radians, float3{ 0, 0, 1 });
	return *this;
}

TextComponent& TextComponent::setRotationDeg(float degrees) {
	return setRotation(glm::radians(degrees));
}

TextComponent& TextComponent::setFontSize(float fontSizePx) {
	_fontSizePx = fontSizePx;
	return *this;
}

float2 TextComponent::getPosition() const noexcept {
	auto transformOpt = _domain->tryGetComponent<scene::components::TransformComponent>(_entity);
	if (transformOpt) {
		return { transformOpt->position.x, transformOpt->position.y };
	}
	return {};
}

float TextComponent::getRotation() const noexcept {
	auto transformOpt = _domain->tryGetComponent<scene::components::TransformComponent>(_entity);
	return transformOpt ? glm::eulerAngles(transformOpt->rotation).z : 0;
}

float TextComponent::getRotationDeg() const noexcept {
	return glm::degrees(getRotation());
}

float2 TextComponent::getAdvance() const noexcept {
	try {
		_assure();
		return { _bottomRight.x, 0 };
	} catch (...) {
		return {};
	}
}

float2 TextComponent::getBaseline() const noexcept {
	try {
		_assure();
		return { _topLeft.x, _baseLine };
	} catch (...) {
		return {};
	}
}

TextComponent& TextComponent::setBaseline(float2 pos) {
	return setPosition({ pos.x, pos.y + _baseLine });
}

void TextComponent::_assure() const {
	if (_text.empty()) {
		throw std::logic_error("Text is empty!");
	} else if (!_face) {
		throw std::logic_error("Font is not set!");
	} else if (!_domain || _entity == ecs::nullEntity ||
			   !_domain->hasComponent<scene::components::TransformComponent>(_entity)) {
		throw std::logic_error("Bad initialization!");
	} else if (_fontSizePx == 0) {
		throw std::logic_error("Font size is 0!");
	}
}

void TextComponent::updateText() {
	_assure();
	float3 pos{};

	auto&& transform = _domain->getComponent<scene::components::TransformComponent>(_entity);

	{
		const auto scale = _fontSizePx / _face->resolution();
		transform.scale = float3{ scale, scale, 0 };
	}

	struct Vertex {
		float3 position{};
		float2 texCoords{};
	};

	auto textVertices = std::vector<Vertex>();
	textVertices.reserve(4 * _text.length());
	auto textIndices = std::vector<u32>();
	textIndices.reserve(6 * _text.length());

	float3 localMin = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), 0 };
	float3 localMax = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), 0 };

	u32 indicesIdx = 0;

	for (u32 i = 0; i != _text.length(); ++i) {
		auto kerning = float3{ i != 0 ? _face->kerning(_text[i - 1], _text[i]) : 0, 0, 0 };

		auto&& gd = _face->glyphData(_text[i]).get();
		if (gd.advanceOnly()) {
			pos += float3{ (gd.advance + kerning.x) * _face->resolution(), 0, 0 };
			continue;
		}

		pos += float3{ kerning.x * _face->resolution(), 0, 0 };

		auto tempPos =
			pos + float3{ gd.planeBounds.left * _face->resolution(), -gd.planeBounds.top * _face->resolution(), 0 };

		textVertices.emplace_back(-tempPos, float2{ gd.uv.first.x, gd.uv.first.y });
		textVertices.emplace_back(float3{ -gd.width, 0, 0 } - tempPos, float2{ gd.uv.second.x, gd.uv.first.y });
		textVertices.emplace_back(float3{ 0, gd.height, 0 } - tempPos, float2{ gd.uv.first.x, gd.uv.second.y });
		textVertices
			.emplace_back(float3{ -gd.width, gd.height, 0 } - tempPos, float2{ gd.uv.second.x, gd.uv.second.y });

		for (const auto& v : textVertices | std::views::reverse | std::views::take(4)) {
			localMin.x = std::min(localMin.x, v.position.x);
			localMin.y = std::min(localMin.y, v.position.y);

			localMax.x = std::max(localMax.x, v.position.x);
			localMax.y = std::max(localMax.y, v.position.y);
		}

		const auto indicesDelta = 4 * indicesIdx;
		textIndices.push_back(2 + indicesDelta);
		textIndices.push_back(0 + indicesDelta);
		textIndices.push_back(1 + indicesDelta);
		textIndices.push_back(3 + indicesDelta);
		textIndices.push_back(2 + indicesDelta);
		textIndices.push_back(1 + indicesDelta);
		++indicesIdx;

		pos += float3{ gd.advance * _face->resolution(), 0, 0 };
	}

	auto mesh = asset::mesh::Mesh::create<Vertex>(textVertices, textIndices);

	struct UniformBuffer {
		Mat4x4 projection;
	};

	// hardcoded, awaiting window.getSize()
	UniformBuffer ubo{ glm::ortho(0.f, (float)1'200, 0.f, (float)600) };

	auto uniformBuffer = gfx::Renderer::getCurrent()
							 ->getBufferManager()
							 ->createBuffer(gfx::BufferType::uniform, &ubo, sizeof(UniformBuffer));

	auto textPipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create({
		.vertexShaderPath = "shaders/vertex_default.glsl",
		.fragmentShaderPath = "shaders/fragment_atlas.glsl",
		.textures = { _face->atlasTexture() },
		.buffers = { uniformBuffer },
	});

	_domain->addComponent<scene::components::MeshComponent>(_entity) =
		scene::components::MeshComponent({ mesh, textPipeline });

	auto tMat = transform.getTransformMatrix();
	auto worldMin = tMat * float4{ localMin.x, localMin.y, localMin.z, 1 };
	auto worldMax = tMat * float4{ localMax.x, localMax.y, localMax.z, 1 };

	_topLeft = float2{ std::min(worldMin.x, worldMax.x), std::max(worldMin.y, worldMax.y) };
	_bottomRight = float2{ std::max(worldMin.x, worldMax.x), std::min(worldMin.y, worldMax.y) };
}

float2 TextComponent::topLeft() const noexcept {
	return _topLeft;
}

float2 TextComponent::bottomRight() const noexcept {
	return _bottomRight;
}

float2 TextComponent::center() const noexcept {
	return (_topLeft + _bottomRight) / 2.f;
}

float TextComponent::rotate(float angle, float2 pivot) {
	_assure();
	auto&& t = _domain->getComponent<scene::components::TransformComponent>(_entity);

	const auto zAx = float3{ 0, 0, 1 };
	const auto rotationQuat = glm::angleAxis(angle, zAx);

	/*auto relative = t.position - float3{ pivot.x, pivot.y, 0 };
	auto rotatedRelative = rotationQuat * relative;*/
	t.position = rotationQuat * (t.position - float3{ pivot.x, pivot.y, 0 }) + float3{ pivot.x, pivot.y, 0 };
	t.rotation = rotationQuat * t.rotation;

	return getRotation();
}

float TextComponent::rotateDeg(float degrees, float2 pivot) {
	return glm::degrees(rotate(glm::radians(degrees), pivot));
}

float TextComponent::rotateDeg(float degrees) {
	return rotateDeg(degrees, center());
}

float TextComponent::rotate(float angle) {
	return rotate(angle, center());
}

} // namespace arch::text
