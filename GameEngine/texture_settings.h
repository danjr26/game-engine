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

	void Set_Border_Color(const ColorRGBAc& in_color);
	void Set_Swizzle(Swizzle in_channel1, Swizzle in_channel2, Swizzle in_channel3, Swizzle in_channel4);
	void Set_Edge_Case(EdgeCase in_sEdgeCase, EdgeCase in_tEdgeCase, EdgeCase in_rEdgeCase);
	void Set_Minify_Filter(FilterMode in_filterMode);
	void Set_Magnify_Filter(FilterMode in_filterMode);

	Texture* Get_Texture();
	ColorRGBAc Get_Border_Color();
	const Swizzle* Get_Swizzle();
	EdgeCase Get_S_Edge_Case();
	EdgeCase Get_T_Edge_Case();
	EdgeCase Get_R_Edge_Case();
	FilterMode Get_Minify_Filter();
	FilterMode Get_Magnify_Filter();

	void Use(TextureSettings* in_currentSettings = nullptr, ubyte in_slot = 0);
};

#endif