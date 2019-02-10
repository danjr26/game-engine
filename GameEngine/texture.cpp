#include "texture.h"
#include "log.h"
#include "game_engine.h"

Texture::Texture(Type in_type, std::string in_filename, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA) :
	mType(in_type),
	mNSamplesMSAA(in_nSamplesMSAA),
	mNBitsPerChannel(in_nBitsPerChannel),
	mFlags(in_flags),
	mActiveSettings(this) {

	cimg_library::CImg<ubyte> image((in_filename).c_str());

	mDimensions = Vector3i(image.width(), image.height(), image.depth());
	mNChannels = image.spectrum();

	mData = new ubyte[mDimensions.Component_Product() * mNChannels];
	cimg_forXYZC(image, x, y, z, c) {
		mData[
			z * mDimensions.X() * mDimensions.Y() * mNChannels +
				y * mDimensions.X() * mNChannels +
				x * mNChannels +
				c] = image(x, y, z, c);
	}

	Finish_Setup();
}

Texture::Texture(Type in_type, Vector3i in_dimensions, ubyte* in_data, ubyte in_nChannels, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA) :
	mType(in_type),
	mDimensions(in_dimensions),
	mNSamplesMSAA(in_nSamplesMSAA),
	mNChannels(in_nChannels),
	mNBitsPerChannel(in_nBitsPerChannel),
	mFlags(in_flags),
	mActiveSettings(this) {

	mData = new ubyte[mDimensions.Component_Product() * mNChannels];
	memcpy(mData, in_data, mDimensions.Component_Product() * mNChannels);

	Finish_Setup();
}

Texture::Texture(Type in_type, Vector3i in_dimensions, ColorRGBAc in_color, ubyte in_nChannels, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA) :
	mType(in_type),
	mDimensions(in_dimensions),
	mNSamplesMSAA(in_nSamplesMSAA),
	mNChannels(in_nChannels),
	mNBitsPerChannel(in_nBitsPerChannel),
	mFlags(in_flags),
	mActiveSettings(this) {

	mData = new ubyte[mDimensions.Component_Product() * mNChannels];
	for (int i = 0; i < mDimensions.Component_Product(); i++) {
		for (int j = 0; j < mNChannels; j++) {
			mData[i * mNChannels + j] = in_color[j];
		}
	}

	Finish_Setup();
}

Texture::Texture(Type in_type, Vector3i in_dimensions, ubyte in_nChannels, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA) :
	mType(in_type),
	mData(nullptr),
	mDimensions(in_dimensions),
	mNSamplesMSAA(in_nSamplesMSAA),
	mNChannels(in_nChannels),
	mNBitsPerChannel(in_nBitsPerChannel),
	mFlags(in_flags),
	mActiveSettings(this) {

	if (mFlags & Flags::readable) {
		mData = new ubyte[mDimensions.Component_Product() * mNChannels];
	}

	Finish_Setup();
}

const ubyte* Texture::Read() {
	if (mFlags & Flags::readable) {
		return mData;
	}
	else {
		throw ProcessFailureException("attempt to read texture marked as non-readable");
	}
}

void Texture::Refresh_From_OpenGL() {
	if (mFlags & Flags::readable) {
		glGetTextureImage(mID, 0, mPixelFormat, GL_UNSIGNED_BYTE, mDimensions.Component_Product() * mNChannels, mData);
	}
	else {
		throw ProcessFailureException("attempt to refresh texture marked as non-readable");
	}
}

void Texture::Use(ubyte in_slot) {
	glActiveTexture(GL_TEXTURE0 + in_slot);
	glBindTexture(mTarget, mID);
}

void Texture::Use_None(ubyte in_slot) {
	glActiveTexture(GL_TEXTURE0 + in_slot);
	glBindTexture(mTarget, 0);
}

Texture::Type Texture::Get_Type() {
	return mType;
}

Vector3i Texture::Get_Dimensions() {
	return mDimensions;
}

ubyte Texture::Get_Number_Channels() {
	return mNChannels;
}

ubyte Texture::Get_Number_Bits_Per_Channel() {
	return mNBitsPerChannel;
}

ubyte Texture::Get_Number_Samples_MSAA() {
	return mNSamplesMSAA;
}

uint Texture::Get_Flags() {
	return mFlags;
}

TextureSettings Texture::Get_Active_Settings() {
	return mActiveSettings;
}

void Texture::Finish_Setup() {
	mTarget = Determine_Target();
	mPixelFormat = Determine_Pixel_Format();
	mInternalFormat = Determine_Internal_Format();

	glCreateTextures(mTarget, 1, &mID);
	mActiveSettings.Use();
	Load_To_OpenGL();

	if (!(mFlags & Flags::readable)) {
		delete[] mData;
		mData = nullptr;
	}
}

void Texture::Load_To_OpenGL() {
	glBindTexture(mTarget, mID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	switch (Determine_Span()) {
	case 1:
		glTexImage1D(mTarget, 0, mInternalFormat, mDimensions.X(), 0, mPixelFormat, GL_UNSIGNED_BYTE, mData);
		break;
	case 2:
		glTexImage2D(mTarget, 0, mInternalFormat, mDimensions.X(), mDimensions.Y(), 0, mPixelFormat, GL_UNSIGNED_BYTE, mData);
		break;
	case 3:
		glTexImage3D(mTarget, 0, mInternalFormat, mDimensions.X(), mDimensions.Y(), mDimensions.Z(), 0, mPixelFormat, GL_UNSIGNED_BYTE, mData);
		break;
	}

	if (mFlags & Flags::mipmaps) {
		glGenerateMipmap(mTarget);
	}

	glBindTexture(mTarget, 0);
}

GLenum Texture::Determine_Pixel_Format() {
	if (mFlags & Flags::depth) {
		return GL_DEPTH_COMPONENT;
	}
	else {
		switch (mNChannels) {
		case 1:
			return GL_RED;
		case 2:
			return GL_RG;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
		}
	}
	return GL_RGBA;
}

GLenum Texture::Determine_Internal_Format() {
	if (mFlags & Flags::depth) {
		switch (mNBitsPerChannel) {
		case 16:
			return GL_DEPTH_COMPONENT16;
		case 24:
			return GL_DEPTH_COMPONENT24;
		case 32:
			return GL_DEPTH_COMPONENT32;
		default:
			throw InvalidArgumentException("invalid internal format");
		}
	}
	else {
		switch (mNBitsPerChannel) {
		case 4:
			switch (mNChannels) {
			case 3:
				return GL_RGB4;
			case 4:
				return GL_RGBA4;
			}
		case 8:
			switch (mNChannels) {
			case 1:
				return GL_R8;
			case 2:
				return GL_RG8;
			case 3:
				return GL_RGB8;
			case 4:
				return GL_RGBA8;
			}
		case 12:
			switch (mNChannels) {
			case 3:
				return GL_RGB12;
			case 4:
				return GL_RGBA12;
			}
		case 16:
			if (mFlags & Flags::hdr) {
				switch (mNChannels) {
				case 1:
					return GL_R16F;
				case 2:
					return GL_RG16F;
				case 3:
					return GL_RGB16F;
				case 4:
					return GL_RGBA16F;
				}
			}
			else {
				switch (mNChannels) {
				case 1:
					return GL_R16;
				case 2:
					return GL_RG16;
				case 3:
					return GL_RGB16;
				case 4:
					return GL_RGBA16;
				}
			}
		case 32:
			if (mFlags & Flags::hdr) {
				switch (mNChannels) {
				case 1:
					return GL_R32F;
				case 2:
					return GL_RG32F;
				case 3:
					return GL_RGB32F;
				case 4:
					return GL_RGBA32F;
				}
			}
		default:
			throw InvalidArgumentException("invalid texture internal format");
		}
	}
}

GLenum Texture::Determine_Target() {
	if (mNSamplesMSAA > 1) {
		switch (mType) {
		case Type::_1d:
		case Type::_1d_array:
			throw InvalidArgumentException("multisampling not supported for 1d textures");
		case Type::_2d:
			return GL_TEXTURE_2D_MULTISAMPLE;
		case Type::_2d_array:
			return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		case Type::_3d:
			throw InvalidArgumentException("multisampling not supported for 3d textures");
		case Type::cubemap_positive_x:
		case Type::cubemap_positive_y:
		case Type::cubemap_positive_z:
		case Type::cubemap_negative_x:
		case Type::cubemap_negative_y:
		case Type::cubemap_negative_z:
			throw InvalidArgumentException("multisampling not supported for cubemap textures");
		}
	}
	else {
		return mType;
	}
	throw InvalidArgumentException("invalid texture target type");
}

ubyte Texture::Determine_Span() {
	switch (mType) {
	case _1d:
		return 1;
	case _1d_array:
	case _2d:
	case cubemap_negative_x:
	case cubemap_negative_y:
	case cubemap_negative_z:
	case cubemap_positive_x:
	case cubemap_positive_y:
	case cubemap_positive_z:
		return 2;
	case _2d_array:
	case _3d:
		return 3;
	}
	throw InvalidArgumentException("invalid texture span");
}
