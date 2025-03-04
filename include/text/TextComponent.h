#pragma once

#include <Font.h>
#include <ecs/Domain.h>

namespace arch::text {

class TextComponent {
public:

	TextComponent(const TextComponent&) noexcept = default;
	TextComponent(TextComponent&&) noexcept = default;

	TextComponent(ecs::Domain& domain, const ecs::Entity entity) noexcept;

	TextComponent& operator=(const TextComponent&) noexcept = default;
	TextComponent& operator=(TextComponent&&) noexcept = default;
	template<class Char>
	TextComponent& operator=(std::basic_string_view<Char> text);
	TextComponent& operator=(font::Face& face) noexcept;

	TextComponent& setFont(std::string_view familyName, std::string_view styleName = "Regular");
	TextComponent& setFont(font::Face& face);

	template<class Char>
	TextComponent& setText(std::basic_string_view<Char> text = {});

	TextComponent& setPosition(float2 pos);
	TextComponent& setBaseline(float2 pos);
	TextComponent& setTopLeft(float2 pos);
	TextComponent& setRotation(float radians);
	TextComponent& setRotationDeg(float degrees);

	TextComponent& setFontSize(float fontSizePx);

	float2 getPosition() const noexcept;
	float2 getBaseline() const noexcept;
	float getRotation() const noexcept;
	float getRotationDeg() const noexcept;

	float2 getAdvance() const noexcept;

	void updateText(bool outline = false);

	float2 topLeft() const noexcept;
	float2 bottomRight() const noexcept;
	float2 center() const noexcept;

	float2 size() const noexcept;

	float rotate(float angle, float2 pivot);
	float rotate(float angle); // pivot = center()
	float rotateDeg(float degrees, float2 pivot);
	float rotateDeg(float degrees); // pivot = center()

private:

	void _assure() const;

	std::u32string _text{};
	font::Face* _face{};
	ecs::Domain* _domain{};
	ecs::Entity _entity = ecs::nullEntity;
	float2 _topLeft{};
	float2 _bottomRight{};
	float _fontSizePx = 12;
	float _baseLine{};
};

} // namespace arch::text

#include "TextComponent.hpp"
