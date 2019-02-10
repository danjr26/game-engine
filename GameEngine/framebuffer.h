#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "render_target.h"
#include "texture.h"
#include "sprite.h"
#include <vector>

class Framebuffer : public RenderTarget {
private:
	GLuint mID;
	std::vector<Texture*> mColorTextures;
	Texture* mDepthTexture;

public:
	Framebuffer(std::vector<Texture*> in_colorTextures, Texture* in_depthTexture);
	~Framebuffer();

	std::vector<Texture*> Get_Color_Textures();
	Texture* Get_Color_Texture(uint in_index = 0);
	Texture* Get_Depth_Texture();

	Vector2i Get_Dimensions() const override final;

protected:
	void _Draw_To_This() override final;
};


#endif