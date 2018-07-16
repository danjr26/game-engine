#ifndef CIRCLE_RENDERER_H
#define CIRCLE_RENDERER_H

#include "sprite.h"

class CircleRenderer : public Sprite {
private:
	double outlineWidth;
	ColorRGBAf outlineColor;
public:
	CircleRenderer(const Vector3d& in_center, double in_radius, const ColorRGBAf& in_color, 
		double in_outlineWidth = 0.0, const ColorRGBAf& in_outlineColor = ColorRGBAf(0, 0, 0, 1));

	void Set_UVs(const Vector2f& in_topLeft, const Vector2f& in_bottomRight) = delete;
	TextureInstance& Texture_Instance() = delete;

	void Render() override;
};

#endif