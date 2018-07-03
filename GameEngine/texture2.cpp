#include "texture2.h"
#include "log.h"
#include "misc.h"
#include "game_engine.h"

Texture2::Texture2(std::string in_path, std::string in_name, ubyte in_nBitsPerChannel, uint in_flags) :
	name(in_name),
	data(nullptr),
	id(0),
	pixelFormat(0),
	internalFormat(0),
	dimensions(),
	nChannels(0),
	nBitsPerChannel(in_nBitsPerChannel),
	flags(in_flags),
	activeSettings(this) {

	cimg_library::CImg<ubyte> image((in_path + in_name).c_str());
	dimensions = Vector2i(image.width(), image.height());
	nChannels = image.spectrum();
	data = new ubyte[dimensions.X() * dimensions.Y() * nChannels];
	cimg_forXYC(image, x, y, c) {
		data[y * dimensions.X() * nChannels + x * nChannels + c] = image(x, y, c);
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	activeSettings.Use();
	pixelFormat = Determine_Pixel_Format();
	internalFormat = Determine_Internal_Format();

	Load_To_OpenGL();

	if (!(flags & Flags::readable)) {
		delete[] data;
		data = nullptr;
	}

	if (name != "") {
		GE.Textures().Add(name, this);
	}
}

Texture2::Texture2(std::string in_name, Vector2i in_dimensions, ubyte in_nChannels, ubyte in_nBitsPerChannel,
	ubyte* in_data, uint in_flags) :

	name(in_name),
	data(nullptr),
	id(0),
	pixelFormat(0),
	internalFormat(0),
	dimensions(in_dimensions),
	nChannels(in_nChannels),
	nBitsPerChannel(in_nBitsPerChannel),
	flags(in_flags),
	activeSettings(this) {

	if (dimensions.X() <= 0 || dimensions.Y() <= 0) {
		throw InvalidArgumentException("invalid texture dimensions");
	}

	if (!Is_Between_Inc<ubyte>(in_nChannels, 1, 4) || (flags & Flags::depth && in_nChannels != 1)) {
		Log::main("error: invalid number of channels");
		exit(-1);
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	activeSettings.Use();
	pixelFormat = Determine_Pixel_Format();
	internalFormat = Determine_Internal_Format();

	if (flags & Flags::readable) {
		uint dataSize = Data_Size();
		data = new ubyte[dataSize];
		memcpy(data, in_data, dataSize);
	}
	else {
		data = in_data;
	}

	Load_To_OpenGL();

	if (!(flags & Flags::readable)) {
		data = nullptr;
	}

	if (name != "") {
		GE.Textures().Add(name, this);
	}
}

Texture2::Texture2(std::string in_name, Vector2i in_dimensions, ubyte in_nChannels, ubyte in_nBitsPerChannel,
	ColorRGBAc in_color, uint in_flags) :

	name(in_name),
	data(nullptr),
	id(0),
	pixelFormat(0),
	internalFormat(0),
	dimensions(in_dimensions),
	nChannels(in_nChannels),
	nBitsPerChannel(in_nBitsPerChannel),
	flags(in_flags),
	activeSettings(this) {

	if (dimensions.X() <= 0 || dimensions.Y() <= 0) {
		Log::main("error: invalid texture dimensions");
		exit(-1);
	}

	if (!Is_Between_Inc<ubyte>(in_nChannels, 1, 4) || (flags & Flags::depth && in_nChannels != 1)) {
		Log::main("error: invalid number of channels");
		exit(-1);
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	activeSettings.Use();
	pixelFormat = Determine_Pixel_Format();
	internalFormat = Determine_Internal_Format();

	uint dataSize = Data_Size();
	data = new ubyte[dataSize];

	for (uint i = 0; i < dimensions.X() * dimensions.Y(); i++) {
		for (ubyte j = 0; j < in_nChannels; j++) {
			data[i * in_nChannels + j] = in_color[j];
		}
	}

	Load_To_OpenGL();

	if (!(flags & Flags::readable)) {
		delete[] data;
		data = nullptr;
	}

	if (name != "") {
		GE.Textures().Add(name, this);
	}
}

Texture2::~Texture2() {
	if (name != "") {
		GE.Textures().Remove(name);
	}

	if (flags & Flags::readable) {
		delete[] data;
	}
	glDeleteTextures(1, &id);
}

const ubyte* Texture2::Read() {
	if (flags & Flags::readable) {
		return data;
	}
	else {
		Log::main("error: cannot read from texture not specified as readable");
		exit(-1);
	}
}

void Texture2::Refresh_From_OpenGL() {
	if (flags & Flags::readable) {
		glGetTextureImage(id, 0, pixelFormat, GL_UNSIGNED_BYTE, Data_Size(), data);
	}
	else {
		Log::main("error: cannot refresh texture not specified as readable");
		exit(-1);
	}
}

void Texture2::Use(ubyte in_slot) {
	glActiveTexture(GL_TEXTURE0 + in_slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2::Use_None(ubyte in_slot) {
	glActiveTexture(GL_TEXTURE0 + in_slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLenum Texture2::Get_Pixel_Format() {
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
}

GLenum Texture2::Determine_Internal_Format() {
	if (flags & Flags::depth) {
		switch (nBitsPerChannel) {
		case 16:
			return GL_DEPTH_COMPONENT16;
		case 24:
			return GL_DEPTH_COMPONENT24;
		case 32:
			return GL_DEPTH_COMPONENT32;
		default:
			Log::main("error: invalid texture internal format");
			exit(-1);
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
			Log::main("error: invalid texture internal format");
			exit(-1);
		}
	}
}

uint Texture2::Data_Size() {
	return (dimensions.X() * dimensions.Y() * nChannels);
}

void Texture2::Load_To_OpenGL() {
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		internalFormat,
		dimensions.X(),
		dimensions.Y(),
		0,
		pixelFormat,
		GL_UNSIGNED_BYTE,
		data
	);

	if (flags & Flags::mipmaps) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

std::string Texture2::Get_Name() {
	return name;
}

Vector2i Texture2::Get_Dimensions() {
	return dimensions;
}

ubyte Texture2::Get_Number_Channels() {
	return nChannels;
}

ubyte Texture2::Get_Number_Bits_Per_Channel() {
	return nBitsPerChannel;
}

uint Texture2::Get_Flags() {
	return flags;
}

TextureSettings Texture2::Get_Active_Settings() {
	return activeSettings;
}
