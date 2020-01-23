#ifndef CIRCLE_RENDERER_H
#define CIRCLE_RENDERER_H

#include "sprite.h"
#include "sphere.h"

class CircleRenderer : public Sprite {
private:
	float mOutlineWidth;
	Color4f mOutlineColor;
public:
	CircleRenderer(const Circled& i_circle = Circled(), const Color4f& i_color = Color4f(1, 1, 1, 1), 
		float i_outlineWidth = 0.0, const Color4f& i_outlineColor = Color4f(0, 0, 0, 1));

	void setUVs(const Vector2f& i_topLeft, const Vector2f& i_bottomRight) = delete;
	TextureInstance& getTextureInstance() = delete;

	void setCircle(const Circled& i_circle);

	void setOutlineColor(const Color4f& i_color);
	Color4f getOutlineColor() const;

	void setOutlineWidth(float i_width);
	float getOutlineWidth() const;

	void render() override;
};

#endif