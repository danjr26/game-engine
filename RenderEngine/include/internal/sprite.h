#ifndef SPRITE_H
#define SPRITE_H

#include "renderable.h"
#include "vertex_pusher.h"
#include "texture_instance.h"

class Sprite : public Renderable {
protected:
	DeepTransform2d mOuterTransform;
	DeepTransform2d mInnerTransform;
	VertexPusher mVertexPusher;
	TextureInstance mTextureInstance;
	Color4f mColor;

public:
	Sprite(
		const DeepTransform2d& i_transform,
		Texture* i_texture = nullptr, 
		const Color4f& i_color = Color4f(1, 1, 1, 1)
	);
	~Sprite();

	void setRectangle(const AABox2d& i_rectangle);
	void setTexture(Texture* i_texture);
	void setUVs(const Vector2f& i_topLeft, const Vector2f& i_bottomRight);
	void setColor(const Color4f& i_color);
	TextureInstance& getTextureInstance();
	double z() const override final;
	bool shouldCull() const override final;
	virtual void render() override;
};

#endif