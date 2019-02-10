#include "texture_settings.h"
#include "texture.h"
#include "log.h"

TextureSettings::TextureSettings(Texture* in_texture) :
mTexture(in_texture),
mBorderColor(255, 255, 255, 255),
mSwizzleMask{GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA},
mSEdgeCase(GL_CLAMP_TO_EDGE),
mTEdgeCase(GL_CLAMP_TO_EDGE),
mREdgeCase(GL_CLAMP_TO_EDGE),
mMinifyFilter(GL_NEAREST),
mMagnifyFilter(GL_NEAREST)
{}

void TextureSettings::Set_Border_Color(const ColorRGBAc& in_color) {
	mBorderColor = in_color;
}

void TextureSettings::Set_Swizzle(Swizzle in_channel1, Swizzle in_channel2, Swizzle in_channel3, Swizzle in_channel4) {
	mSwizzleMask[0] = (GLint)in_channel1;
	mSwizzleMask[1] = (GLint)in_channel2;
	mSwizzleMask[2] = (GLint)in_channel3;
	mSwizzleMask[3] = (GLint)in_channel4;
}

void TextureSettings::Set_Edge_Case(EdgeCase in_sEdgeCase, EdgeCase in_tEdgeCase, EdgeCase in_rEdgeCase) {
	mSEdgeCase = (GLenum)in_sEdgeCase;
	mTEdgeCase = (GLenum)in_tEdgeCase;
	mREdgeCase = (GLenum)in_rEdgeCase;
}

void TextureSettings::Set_Minify_Filter(FilterMode in_filterMode) {
	if (!(mTexture->Get_Flags() & Texture::Flags::mipmaps)) {
		if (in_filterMode == FilterMode::none || in_filterMode == FilterMode::z_bilinear) {
			mMinifyFilter = GL_NEAREST;
		}
		else {
			mMinifyFilter = GL_LINEAR;
		}
	}
	else {
		mMinifyFilter = (GLenum)in_filterMode;
	}
}

void TextureSettings::Set_Magnify_Filter(FilterMode in_filterMode) {
	if (in_filterMode == FilterMode::none || in_filterMode == FilterMode::z_bilinear) {
		mMagnifyFilter = GL_NEAREST;
	}
	else {
		mMagnifyFilter = GL_LINEAR;
	}
}

Texture* TextureSettings::Get_Texture() {
	return mTexture;
}

ColorRGBAc TextureSettings::Get_Border_Color() {
	return mBorderColor;
}

const TextureSettings::Swizzle* TextureSettings::Get_Swizzle() {
	return (Swizzle*) mSwizzleMask;
}

TextureSettings::EdgeCase TextureSettings::Get_S_Edge_Case() {
	return (EdgeCase) mSEdgeCase;
}

TextureSettings::EdgeCase TextureSettings::Get_T_Edge_Case() {
	return (EdgeCase) mTEdgeCase;
}

TextureSettings::EdgeCase TextureSettings::Get_R_Edge_Case() {
	return (EdgeCase) mREdgeCase;
}

TextureSettings::FilterMode TextureSettings::Get_Minify_Filter() {
	switch (mMinifyFilter) {
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
	if (mMagnifyFilter == GL_LINEAR) {
		return FilterMode::bilinear;
	}
	else {
		return FilterMode::none;
	}
}

void TextureSettings::Use(TextureSettings* in_currentSettings, ubyte in_slot) {
	mTexture->Use(in_slot);
	GLenum target = mTexture->mTarget;

	if (in_currentSettings == nullptr) {
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, ColorRGBAf(mBorderColor).Pointer());
		glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, mSwizzleMask);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, mSEdgeCase);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, mTEdgeCase);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, mREdgeCase);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, mMinifyFilter);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mMagnifyFilter);
	}
	else {
		if (mTexture != in_currentSettings->Get_Texture()) {
			throw InvalidArgumentException("texture settings parent mismatch");
		}
		if (in_currentSettings->mBorderColor != mBorderColor) {
			glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, ColorRGBAf(mBorderColor).Pointer());
		}

		if (in_currentSettings->mSwizzleMask[0] != mSwizzleMask[0] ||
			in_currentSettings->mSwizzleMask[1] != mSwizzleMask[1] ||
			in_currentSettings->mSwizzleMask[2] != mSwizzleMask[2] ||
			in_currentSettings->mSwizzleMask[3] != mSwizzleMask[3]) {

			glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, mSwizzleMask);
		}

		if (in_currentSettings->mSEdgeCase != mSEdgeCase) {
			glTexParameteri(target, GL_TEXTURE_WRAP_S, mSEdgeCase);
		}

		if (in_currentSettings->mTEdgeCase != mTEdgeCase) {
			glTexParameteri(target, GL_TEXTURE_WRAP_T, mTEdgeCase);
		}

		if (in_currentSettings->mREdgeCase != mREdgeCase) {
			glTexParameteri(target, GL_TEXTURE_WRAP_R, mREdgeCase);
		}

		if (in_currentSettings->mMinifyFilter != mMinifyFilter) {
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, mMinifyFilter);
		}

		if (in_currentSettings->mMagnifyFilter != mMagnifyFilter) {
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mMagnifyFilter);
		}
	}

	mTexture->mActiveSettings = (*this);
}


