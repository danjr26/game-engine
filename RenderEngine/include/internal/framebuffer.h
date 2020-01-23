#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "render_target.h"
#include "texture.h"
#include "sprite.h"
#include <vector>

class Framebuffer : public RenderTarget {
private:
	GLuint mId;
	std::vector<Texture*> mColorTextures;
	Texture* mDepthTexture;

public:
	Framebuffer(std::vector<Texture*> i_colorTextures, Texture* i_depthTexture);
	~Framebuffer();

	std::vector<Texture*> getColorTextures();
	Texture* getColorTexture(uint i_index = 0);
	Texture* getDepthTexture();

	Vector2ui getDimensions() const override final;

protected:
	void _drawToThis() override final;
};


#endif