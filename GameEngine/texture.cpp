#include "texture.h"
#include "rapidxml-1.13/rapidxml.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "log.h"
#include "game_engine.h"

Texture::Texture(Type in_type, std::string in_filename, ubyte in_nBitsPerChannel, uint in_flags, ubyte in_nSamplesMSAA) :
	mType(in_type),
	mNSamplesMSAA(in_nSamplesMSAA),
	mNBitsPerChannel(in_nBitsPerChannel),
	mFlags(in_flags),
	mActiveSettings(this) {

	cimg_library::CImg<ubyte> image((in_filename).c_str());

	mDimensions = Vector3i(image.width(), image.height(), image.depth());
	mNChannels = image.spectrum();

	mData = new ubyte[mDimensions.componentProduct() * mNChannels];
	cimg_forXYZC(image, x, y, z, c) {
		mData[
			z * mDimensions.x() * mDimensions.y() * mNChannels +
				y * mDimensions.x() * mNChannels +
				x * mNChannels +
				c] = image(x, y, z, c);
	}

	finishSetup();
}

Texture::Texture(Type in_type, Vector3i in_dimensions, ubyte* in_data, ubyte in_nChannels, ubyte in_nBitsPerChannel, uint in_flags, ubyte in_nSamplesMSAA) :
	mType(in_type),
	mDimensions(in_dimensions),
	mNSamplesMSAA(in_nSamplesMSAA),
	mNChannels(in_nChannels),
	mNBitsPerChannel(in_nBitsPerChannel),
	mFlags(in_flags),
	mActiveSettings(this) {

	mData = new ubyte[mDimensions.componentProduct() * mNChannels];
	memcpy(mData, in_data, mDimensions.componentProduct() * mNChannels);

	finishSetup();
}

Texture::Texture(Type in_type, Vector3i in_dimensions, ColorRGBAc in_color, ubyte in_nChannels, ubyte in_nBitsPerChannel, uint in_flags, ubyte in_nSamplesMSAA) :
	mType(in_type),
	mDimensions(in_dimensions),
	mNSamplesMSAA(in_nSamplesMSAA),
	mNChannels(in_nChannels),
	mNBitsPerChannel(in_nBitsPerChannel),
	mFlags(in_flags),
	mActiveSettings(this) {

	mData = new ubyte[mDimensions.componentProduct() * mNChannels];
	for (int i = 0; i < mDimensions.componentProduct(); i++) {
		for (int j = 0; j < mNChannels; j++) {
			mData[i * mNChannels + j] = in_color[j];
		}
	}

	finishSetup();
}

Texture::Texture(Type in_type, Vector3i in_dimensions, ubyte in_nChannels, ubyte in_nBitsPerChannel, uint in_flags, ubyte in_nSamplesMSAA) :
	mType(in_type),
	mData(nullptr),
	mDimensions(in_dimensions),
	mNSamplesMSAA(in_nSamplesMSAA),
	mNChannels(in_nChannels),
	mNBitsPerChannel(in_nBitsPerChannel),
	mFlags(in_flags),
	mActiveSettings(this) {

	if (mFlags & Flags::readable) {
		mData = new ubyte[mDimensions.componentProduct() * mNChannels];
	}

	finishSetup();
}

Texture::~Texture() {
	if (mFlags & Flags::readable) {
		delete[] mData;
	}
	glDeleteTextures(1, &mID);
}

const ubyte* Texture::read() {
	if (mFlags & Flags::readable) {
		return mData;
	}
	else {
		throw ProcessFailureException("attempt to read texture marked as non-readable");
	}
}

void Texture::refreshFromOpenGL() {
	if (mFlags & Flags::readable) {
		glGetTextureImage(mID, 0, mPixelFormat, GL_UNSIGNED_BYTE, mDimensions.componentProduct() * mNChannels, mData);
	}
	else {
		throw ProcessFailureException("attempt to refresh texture marked as non-readable");
	}
}

void Texture::use(ubyte in_slot) {
	glActiveTexture(GL_TEXTURE0 + in_slot);
	glBindTexture(mTarget, mID);
}

void Texture::useNone(ubyte in_slot) {
	glActiveTexture(GL_TEXTURE0 + in_slot);
	glBindTexture(mTarget, 0);
}

Texture::Type Texture::getType() {
	return mType;
}

Vector3i Texture::getDimensions() {
	return mDimensions;
}

ubyte Texture::getNumberChannels() {
	return mNChannels;
}

ubyte Texture::getNumberBitsPerChannel() {
	return mNBitsPerChannel;
}

ubyte Texture::getNumberSamplesMSAA() {
	return mNSamplesMSAA;
}

uint Texture::getFlags() {
	return mFlags;
}

const TextureSettings& Texture::getActiveSettings() {
	return mActiveSettings;
}

void Texture::loadXMLList(std::string in_filename) {
	std::ifstream file(in_filename);
	if (!file.is_open()) {
		Log::main(std::string("error: cannot open file '") + in_filename + "'");
		GE.quit();
	}

	file.seekg(0, file.end);
	uint length = (uint)file.tellg();
	file.seekg(0, file.beg);

	char* buffer = new char[length];
	for (uint i = 0; i < length; i++) {
		buffer[i] = '\0';
	}
	file.read(buffer, length);
	file.close();

	rapidxml::xml_document<char> doc;
	doc.parse<0>(buffer);

	auto masterNode = doc.first_node("TextureList");
	if (!masterNode) {
		Log::main(std::string("error: invalid texture list file '") + in_filename + "'");
		GE.quit();
	}

	std::unordered_map<std::string, uint> flagMap = {
		{"none", Flags::none},
		{"readable", Flags::readable},
		{"mipmaps", Flags::mipmaps},
		{"hdr", Flags::hdr},
		{"depth", Flags::depth}
	};

	for (auto node = masterNode->first_node("Texture2"); node; node = node->next_sibling("Texture2")) {
		auto nameAttribute = node->first_attribute("name");
		auto fileAttribute = node->first_attribute("file");
		auto flagsAttribute = node->first_attribute("flags");

		if (!nameAttribute || !fileAttribute) {
			Log::main(std::string("error: invalid shader list file '") + in_filename + "'");
			GE.quit();
		}

		uint flags = 0;
		if (flagsAttribute) {
			std::stringstream stream;
			stream << flagsAttribute->value();
			std::string flag;
			while (std::getline(stream, flag, ',')) {
				flags |= flagMap.at(flag);
			}
		}

		Texture* texture = new Texture(Type::_2d, fileAttribute->value(), 8, flags);
		GE.assets().add(nameAttribute->value(), texture);
	}

	delete[] buffer;
}

void Texture::finishSetup() {
	mTarget = determineTarget();
	mPixelFormat = determinePixelFormat();
	mInternalFormat = determineInternalFormat();

	glCreateTextures(mTarget, 1, &mID);
	mActiveSettings.use();
	loadToOpenGL();

	if (!(mFlags & Flags::readable)) {
		delete[] mData;
		mData = nullptr;
	}
}

void Texture::loadToOpenGL() {
	glBindTexture(mTarget, mID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	switch (determineSpan()) {
	case 1:
		glTexImage1D(mTarget, 0, mInternalFormat, mDimensions.x(), 0, mPixelFormat, GL_UNSIGNED_BYTE, mData);
		break;
	case 2:
		glTexImage2D(mTarget, 0, mInternalFormat, mDimensions.x(), mDimensions.y(), 0, mPixelFormat, GL_UNSIGNED_BYTE, mData);
		break;
	case 3:
		glTexImage3D(mTarget, 0, mInternalFormat, mDimensions.x(), mDimensions.y(), mDimensions.z(), 0, mPixelFormat, GL_UNSIGNED_BYTE, mData);
		break;
	}

	if (mFlags & Flags::mipmaps) {
		glGenerateMipmap(mTarget);
	}

	glBindTexture(mTarget, 0);
}

GLenum Texture::determinePixelFormat() {
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

GLenum Texture::determineInternalFormat() {
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

GLenum Texture::determineTarget() {
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

ubyte Texture::determineSpan() {
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
