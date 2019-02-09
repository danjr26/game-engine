#include "texture_settings.h"
#include "texture.h"
#include "log.h"

TextureSettings::TextureSettings(Texture* in_texture) :
texture(in_texture),
borderColor(255, 255, 255, 255),
swizzleMask{GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA},
sEdgeCase(GL_CLAMP_TO_EDGE),
tEdgeCase(GL_CLAMP_TO_EDGE),
rEdgeCase(GL_CLAMP_TO_EDGE),
minifyFilter(GL_NEAREST),
magnifyFilter(GL_NEAREST)
{}

void TextureSettings::Set_Border_Color(const ColorRGBAc& in_color) {
	borderColor = in_color;
}

void TextureSettings::Set_Swizzle(Swizzle in_channel1, Swizzle in_channel2, Swizzle in_channel3, Swizzle in_channel4) {
	swizzleMask[0] = (GLint)in_channel1;
	swizzleMask[1] = (GLint)in_channel2;
	swizzleMask[2] = (GLint)in_channel3;
	swizzleMask[3] = (GLint)in_channel4;
}

void TextureSettings::Set_Edge_Case(EdgeCase in_sEdgeCase, EdgeCase in_tEdgeCase, EdgeCase in_rEdgeCase) {
	sEdgeCase = (GLenum)in_sEdgeCase;
	tEdgeCase = (GLenum)in_tEdgeCase;
	rEdgeCase = (GLenum)in_rEdgeCase;
}

void TextureSettings::Set_Minify_Filter(FilterMode in_filterMode) {
	if (!(texture->Get_Flags() & Texture::Flags::mipmaps)) {
		if (in_filterMode == FilterMode::none || in_filterMode == FilterMode::z_bilinear) {
			minifyFilter = GL_NEAREST;
		}
		else {
			minifyFilter = GL_LINEAR;
		}
	}
	else {
		minifyFilter = (GLenum)in_filterMode;
	}
}

void TextureSettings::Set_Magnify_Filter(FilterMode in_filterMode) {
	if (in_filterMode == FilterMode::none || in_filterMode == FilterMode::z_bilinear) {
		magnifyFilter = GL_NEAREST;
	}
	else {
		magnifyFilter = GL_LINEAR;
	}
}

Texture* TextureSettings::Get_Texture() {
	return texture;
}

ColorRGBAc TextureSettings::Get_Border_Color() {
	return borderColor;
}

const TextureSettings::Swizzle* TextureSettings::Get_Swizzle() {
	return (Swizzle*) swizzleMask;
}

TextureSettings::EdgeCase TextureSettings::Get_S_Edge_Case() {
	return (EdgeCase) sEdgeCase;
}

TextureSettings::EdgeCase TextureSettings::Get_T_Edge_Case() {
	return (EdgeCase) tEdgeCase;
}

TextureSettings::EdgeCase TextureSettings::Get_R_Edge_Case() {
	return (EdgeCase) rEdgeCase;
}

TextureSettings::FilterMode TextureSettings::Get_Minify_Filter() {
	switch (minifyFilter) {
	case GL_NEAREST:
	case GL_NEAREST_MIPMAP_NEAREST:
		return FilterMode::none;
	case GL_LINEAR:
	case GL_LINEAR_MIPMAP_NEAREST:
		return FilterMode::bilinear;
	case GL_NEAREST_MIPMAP_LINEAR:
		return FilterMode::z_bilinear;
	case GL_LINEAR_MIPMAP_LINEAR:
		return FilterMode::trilinear;
	default:
		return FilterMode::none;
	}
}

TextureSettings::FilterMode TextureSettings::Get_Magnify_Filter() {
	if (magnifyFilter == GL_LINEAR) {
		return FilterMode::bilinear;
	}
	else {
		return FilterMode::none;
	}
}

void TextureSettings::Use(TextureSettings* in_currentSettings, ubyte in_slot) {
	texture->Use(in_slot);
	GLenum target = texture->target;

	if (in_currentSettings == nullptr) {
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, ColorRGBAf(borderColor).Pointer());
		glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, sEdgeCase);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, tEdgeCase);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, rEdgeCase);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minifyFilter);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magnifyFilter);
	}
	else {
		if (texture != in_currentSettings->Get_Texture()) {
			throw InvalidArgumentException("texture settings parent mismatch");
		}
		if (in_currentSettings->borderColor != borderColor) {
			glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, ColorRGBAf(borderColor).Pointer());
		}

		if (in_currentSettings->swizzleMask[0] != swizzleMask[0] ||
			in_currentSettings->swizzleMask[1] != swizzleMask[1] ||
			in_currentSettings->swizzleMask[2] != swizzleMask[2] ||
			in_currentSettings->swizzleMask[3] != swizzleMask[3]) {

			glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}

		if (in_currentSettings->sEdgeCase != sEdgeCase) {
			glTexParameteri(target, GL_TEXTURE_WRAP_S, sEdgeCase);
		}

		if (in_currentSettings->tEdgeCase != tEdgeCase) {
			glTexParameteri(target, GL_TEXTURE_WRAP_T, tEdgeCase);
		}

		if (in_currentSettings->rEdgeCase != rEdgeCase) {
			glTexParameteri(target, GL_TEXTURE_WRAP_R, rEdgeCase);
		}

		if (in_currentSettings->minifyFilter != minifyFilter) {
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minifyFilter);
		}

		if (in_currentSettings->magnifyFilter != magnifyFilter) {
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magnifyFilter);
		}
	}

	texture->activeSettings = (*this);
}


