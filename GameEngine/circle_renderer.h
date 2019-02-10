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

	void setUVs(const Vector2f& in_topLeft, const Vector2f& in_bottomRight) = delete;
	TextureInstance& getTextureInstance() = delete;

	void setOutlineColor(const ColorRGBAf& in_color);
	ColorRGBAf getOutlineColor() const;

	void setOutlineWidth(float in_width);
	float getOutlineWidth() const;

	void render() override;
};

#endif