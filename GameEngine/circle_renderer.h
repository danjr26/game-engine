#ifndef CIRCLE_RENDERER_H
#define CIRCLE_RENDERER_H

#include "sprite.h"
#include "sphere.h"

class CircleRenderer : public Sprite {
private:
	float mOutlineWidth;
	ColorRGBAf mOutlineColor;
public:
	CircleRenderer(const Circled& in_circle, const ColorRGBAf& in_color, 
		float in_outlineWidth = 0.0, const ColorRGBAf& in_outlineColor = ColorRGBAf(0, 0, 0, 1));

	void Set_UVs(const Vector2f& in_topLeft, const Vector2f& in_bottomRight) = delete;
	TextureInstance& Texture_Instance() = delete;

	void Set_Outline_Color(const ColorRGBAf& in_color);
	ColorRGBAf Get_Outline_Color() const;

	void Set_Outline_Width(float in_width);
	float Get_Outline_Width() const;

	void Render() override;
};

#endif