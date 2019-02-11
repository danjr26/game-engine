#ifndef TEXTURE_SETTINGS_H
#define TEXTURE_SETTINGS_H

#include <GL/glew.h>
#include "color.h"

class Texture;

class TextureSettings {
public:
	enum class FilterMode : GLenum {
		none = GL_NEAREST_MIPMAP_NEAREST,
		bilinear = GL_LINEAR_MIPMAP_NEAREST,
		z_bilinear = GL_NEAREST_MIPMAP_LINEAR,
		trilinear = GL_LINEAR_MIPMAP_LINEAR
	};

	enum class EdgeCase : GLenum {
		keep_edge_value = GL_CLAMP_TO_EDGE,
		use_specified_value = GL_CLAMP_TO_BORDER,
		repeat_regular = GL_REPEAT,
		repeat_reversed = GL_MIRRORED_REPEAT
	};

	enum class Swizzle : GLint {
		channel_1 = GL_RED,
		channel_2 = GL_GREEN,
		channel_3 = GL_BLUE,
		channel_4 = GL_ALPHA,
		zero = GL_ZERO,
		one = GL_ONE
	};

private:
	Texture* mTexture;
	ColorRGBAc mBorderColor;
	GLint mSwizzleMask[4];
	GLenum mSEdgeCase;
	GLenum mTEdgeCase;
	GLenum mREdgeCase;
	GLenum mMinifyFilter;
	GLenum mMagnifyFilter;

public:
	TextureSettings(Texture* in_texture);

	void setTexture(Texture* in_texture);
	void setBorderColor(const ColorRGBAc& in_color);
	void setSwizzle(Swizzle in_channel1, Swizzle in_channel2, Swizzle in_channel3, Swizzle in_channel4);
	void setEdgeCase(EdgeCase in_sEdgeCase, EdgeCase in_tEdgeCase, EdgeCase in_rEdgeCase);
	void setMinifyFilter(FilterMode in_filterMode);
	void setMagnifyFilter(FilterMode in_filterMode);

	Texture* getTexture();
	ColorRGBAc getBorderColor();
	const Swizzle* getSwizzle();
	EdgeCase getSEdgeCase();
	EdgeCase getTEdgeCase();
	EdgeCase getREdgeCase();
	FilterMode getMinifyFilter();
	FilterMode getMagnifyFilter();

	void use(TextureSettings* in_currentSettings = nullptr, ubyte in_slot = 0);
};

#endif