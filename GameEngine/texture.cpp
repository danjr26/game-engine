#include "texture.h"
#include "log.h"
#include "game_engine.h"

Texture::Texture(Type in_type, std::string in_filename, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA) :
	type(in_type),
	nSamplesMSAA(in_nSamplesMSAA),
	nBitsPerChannel(in_nBitsPerChannel),
	flags(in_flags),
	activeSettings(this) {

	cimg_library::CImg<ubyte> image((in_filename).c_str());

	dimensions = Vector3i(image.width(), image.height(), image.depth());
	nChannels = image.spectrum();

	data = new ubyte[dimensions.Component_Product() * nChannels];
	cimg_forXYZC(image, x, y, z, c) {
		data[
			z * dimensions.X() * dimensions.Y() * nChannels +
				y * dimensions.X() * nChannels +
				x * nChannels +
				c] = image(x, y, z, c);
	}

	Finish_Setup();
}

Texture::Texture(Type in_type, Vector3i in_dimensions, ubyte* in_data, ubyte in_nChannels, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA) :
	type(in_type),
	dimensions(in_dimensions),
	nSamplesMSAA(in_nSamplesMSAA),
	nChannels(in_nChannels),
	nBitsPerChannel(in_nBitsPerChannel),
	flags(in_flags),
	activeSettings(this) {

	data = new ubyte[dimensions.Component_Product() * nChannels];
	memcpy(data, in_data, dimensions.Component_Product() * nChannels);

	Finish_Setup();
}

Texture::Texture(Type in_type, Vector3i in_dimensions, ColorRGBAc in_color, ubyte in_nChannels, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA) :
	type(in_type),
	dimensions(in_dimensions),
	nSamplesMSAA(in_nSamplesMSAA),
	nChannels(in_nChannels),
	nBitsPerChannel(in_nBitsPerChannel),
	flags(in_flags),
	activeSettings(this) {

	data = new ubyte[dimensions.Component_Product() * nChannels];
	for (int i = 0; i < dimensions.Component_Product(); i++) {
		for (int j = 0; j < nChannels; j++) {
			data[i * nChannels + j] = in_color[j];
		}
	}

	Finish_Setup();
}

Texture::Texture(Type in_type, Vector3i in_dimensions, ubyte in_nChannels, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA) :
	type(in_type),
	data(nullptr),
	dimensions(in_dimensions),
	nSamplesMSAA(in_nSamplesMSAA),
	nChannels(in_nChannels),
	nBitsPerChannel(in_nBitsPerChannel),
	flags(in_flags),
	activeSettings(this) {

	if (flags & Flags::readable) {
		data = new ubyte[dimensions.Component_Product() * nChannels];
	}

	Finish_Setup();
}

const ubyte* Texture::Read() {
	if (flags & Flags::readable) {
		return data;
	}
	else {
		throw ProcessFailureException("attempt to read texture marked as non-readable");
	}
}

void Texture::Refresh_From_OpenGL() {
	if (flags & Flags::readable) {
		glGetTextureImage(id, 0, pixelFormat, GL_UNSIGNED_BYTE, dimensions.Component_Product() * nChannels, data);
	}
	else {
		throw ProcessFailureException("attempt to refresh texture marked as non-readable");
	}
}

void Texture::Use(ubyte in_slot) {
	glActiveTexture(GL_TEXTURE0 + in_slot);
	glBindTexture(target, id);
}

void Texture::Use_None(ubyte in_slot) {
	glActiveTexture(GL_TEXTURE0 + in_slot);
	glBindTexture(target, 0);
}

Texture::Type Texture::Get_Type() {
	return type;
}

Vector3i Texture::Get_Dimensions() {
	return dimensions;
}

ubyte Texture::Get_Number_Channels() {
	return nChannels;
}

ubyte Texture::Get_Number_Bits_Per_Channel() {
	return nBitsPerChannel;
}

ubyte Texture::Get_Number_Samples_MSAA() {
	return nSamplesMSAA;
}

uint Texture::Get_Flags() {
	return flags;
}

TextureSettings Texture::Get_Active_Settings() {
	return activeSettings;
}

void Texture::Finish_Setup() {
	target = Determine_Target();
	pixelFormat = Determine_Pixel_Format();
	internalFormat = Determine_Internal_Format();

	glCreateTextures(target, 1, &id);
	activeSettings.Use();
	Load_To_OpenGL();

	if (!(flags & Flags::readable)) {
		delete[] data;
		data = nullptr;
	}
}

void Texture::Load_To_OpenGL() {
	glBindTexture(target, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	switch (Determine_Span()) {
	case 1:
		glTexImage1D(target, 0, internalFormat, dimensions.X(), 0, pixelFormat, GL_UNSIGNED_BYTE, data);
		break;
	case 2:
		glTexImage2D(target, 0, internalFormat, dimensions.X(), dimensions.Y(), 0, pixelFormat, GL_UNSIGNED_BYTE, data);
		break;
	case 3:
		glTexImage3D(target, 0, internalFormat, dimensions.X(), dimensions.Y(), dimensions.Z(), 0, pixelFormat, GL_UNSIGNED_BYTE, data);
		break;
	}

	if (flags & Flags::mipmaps) {
		glGenerateMipmap(target);
	}

	glBindTexture(target, 0);
}

GLenum Texture::Determine_Pixel_Format() {
	if (flags & Flags::depth) {
		return GL_DEPTH_COMPONENT;
	}
	else {
		switch (nChannels) {
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
	if (flags & Flags::depth) {
		switch (nBitsPerChannel) {
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
		switch (nBitsPerChannel) {
		case 4:
			switch (nChannels) {
			case 3:
				return GL_RGB4;
			case 4:
				return GL_RGBA4;
			}
		case 8:
			switch (nChannels) {
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
			switch (nChannels) {
			case 3:
				return GL_RGB12;
			case 4:
				return GL_RGBA12;
			}
		case 16:
			if (flags & Flags::hdr) {
				switch (nChannels) {
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
				switch (nChannels) {
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
			if (flags & Flags::hdr) {
				switch (nChannels) {
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
	if (nSamplesMSAA > 1) {
		switch (type) {
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
		return type;
	}
	throw InvalidArgumentException("invalid texture target type");
}

ubyte Texture::Determine_Span() {
	switch (type) {
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
