#include "../include/internal/texture.h"
#define cimg_use_png
#include "CImg.h"

/*
Texture::Texture(TextureType i_type, PixelFormat i_localFormat, PixelFormat i_gpuFormat) :
mId(0),
mType(i_type),
mLocalFormat(i_localFormat),
mGPUFormat(i_gpuFormat),
mDimen(),

{}

Texture::Texture(Type i_type, std::string i_filename, ubyte i_nBitsPerChannel, uint i_flags, ubyte i_nSamplesMSAA) :
	mType(i_type),
	mNSamplesMSAA(i_nSamplesMSAA),
	mNBitsPerChannel(i_nBitsPerChannel),
	mFlags(i_flags),
	mActiveSettings(this) {

	cimg_library::CImg<ubyte> image((i_filename).c_str());

	mDimensions = Vector3ui(image.width(), image.height(), image.depth());
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

Texture::Texture(Type i_type, Vector3ui i_dimensions, ubyte* i_data, ubyte i_nChannels, ubyte i_nBitsPerChannel, uint i_flags, ubyte i_nSamplesMSAA) :
	mType(i_type),
	mDimensions(i_dimensions),
	mNSamplesMSAA(i_nSamplesMSAA),
	mNChannels(i_nChannels),
	mNBitsPerChannel(i_nBitsPerChannel),
	mFlags(i_flags),
	mActiveSettings(this) {

	mData = new ubyte[mDimensions.componentProduct() * mNChannels];
	memcpy(mData, i_data, mDimensions.componentProduct() * mNChannels);

	finishSetup();
}

Texture::Texture(Type i_type, Vector3ui i_dimensions, Color4c i_color, ubyte i_nChannels, ubyte i_nBitsPerChannel, uint i_flags, ubyte i_nSamplesMSAA) :
	mType(i_type),
	mDimensions(i_dimensions),
	mNSamplesMSAA(i_nSamplesMSAA),
	mNChannels(i_nChannels),
	mNBitsPerChannel(i_nBitsPerChannel),
	mFlags(i_flags),
	mActiveSettings(this) {

	mData = new ubyte[mDimensions.componentProduct() * mNChannels];
	for (int i = 0; i < mDimensions.componentProduct(); i++) {
		for (int j = 0; j < mNChannels; j++) {
			mData[i * mNChannels + j] = i_color[j];
		}
	}

	finishSetup();
}

Texture::Texture(Type i_type, Vector3ui i_dimensions, ubyte i_nChannels, ubyte i_nBitsPerChannel, uint i_flags, ubyte i_nSamplesMSAA) :
	mType(i_type),
	mData(nullptr),
	mDimensions(i_dimensions),
	mNSamplesMSAA(i_nSamplesMSAA),
	mNChannels(i_nChannels),
	mNBitsPerChannel(i_nBitsPerChannel),
	mFlags(i_flags),
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
	glDeleteTextures(1, &mId);
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
		glGetTextureImage(mId, 0, mPixelFormat, GL_UNSIGNED_BYTE, mDimensions.componentProduct() * mNChannels, mData);
	}
	else {
		throw ProcessFailureException("attempt to refresh texture marked as non-readable");
	}
}

void Texture::use(ubyte i_slot) {
	glActiveTexture(GL_TEXTURE0 + i_slot);
	glBindTexture(mTarget, mId);
}

void Texture::useNone(ubyte i_slot) {
	glActiveTexture(GL_TEXTURE0 + i_slot);
	glBindTexture(mTarget, 0);
}

Texture::Type Texture::getType() {
	return mType;
}

Vector3ui Texture::getDimensions() {
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

void Texture::loadXMLList(std::string i_filename) {
	std::ifstream file(i_filename);
	if (!file.is_open()) {
		Log::main(std::string("error: cannot open file '") + i_filename + "'");
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
		Log::main(std::string("error: invalid texture list file '") + i_filename + "'");
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
			Log::main(std::string("error: invalid shader list file '") + i_filename + "'");
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

	glCreateTextures(mTarget, 1, &mId);
	mActiveSettings.use();
	loadToOpenGL();

	if (!(mFlags & Flags::readable)) {
		delete[] mData;
		mData = nullptr;
	}
}

void Texture::loadToOpenGL() {
	glBindTexture(mTarget, mId);
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

*/

Texture::Texture(TextureType i_type, PixelFormat i_localFormat, PixelFormat i_gpuFormat, const Vector4ui& i_dimen) :
mId(0),
mType(i_type),
mLocalFormat(i_localFormat),
mGPUFormat(i_gpuFormat),
mDimen(i_dimen),
mData(nullptr) {
	if (mType.isCube() && i_dimen.w() != 6 || !mType.isCube() && i_dimen.w() != 1) fail();
}

Texture::~Texture() {
	clearGPU();
	clearLocal();
}

void* Texture::getData(uint i_arrayIndex, CubeMapFace i_cubeFace) {
	Vector3ui coords(0, i_arrayIndex, mType.isCube() ? i_cubeFace.toIndex() : 0);
	Vector3ui dimen(mLocalFormat.getSize() * mDimen.x() * mDimen.y(), mDimen.z(), mDimen.w());
	return mData + coords.getAsCoords(dimen);
}

void Texture::clearLocal() {
	if (mData) {
		delete[] mData;
		mData = nullptr;
	}
}

void Texture::clearGPU() {
	if (mId) {
		glDeleteTextures(1, &mId);
		mId = 0;
	}
}

void Texture::loadFile(const std::string& i_filename, uint i_arrayIndex, CubeMapFace i_cubeFace) {
	switch (mLocalFormat.getPushableType().mValue) {
	case PushableType::Value::i8: _loadFile<sbyte>(i_filename, i_arrayIndex, i_cubeFace);
	case PushableType::Value::ui8: _loadFile<ubyte>(i_filename, i_arrayIndex, i_cubeFace);
	case PushableType::Value::i16: _loadFile<short>(i_filename, i_arrayIndex, i_cubeFace);
	case PushableType::Value::ui16: _loadFile<ushort>(i_filename, i_arrayIndex, i_cubeFace);
	case PushableType::Value::f32: _loadFile<float>(i_filename, i_arrayIndex, i_cubeFace);
	default: fail();
	}
}

void Texture::loadData(void* i_data, uint i_arrayIndex, CubeMapFace i_cubeFace) {
	for (ubyte i = 0; i < 4; i++) {
		if (mDimen[i] == 0) fail();
	}
	if (mData == nullptr) {
		mData = new ubyte[getDataSize()];
	}
	memcpy(getData(i_arrayIndex, i_cubeFace), i_data, getDataSize());
}

void Texture::loadBlank(void* i_color, uint i_arrayIndex, CubeMapFace i_cubeFace) {
	for (ubyte i = 0; i < 4; i++) {
		if (mDimen[i] == 0) fail();
	}
	if (mData == nullptr) {
		mData = new ubyte[getDataSize()];
	}
	ubyte* data = (ubyte*)getData(i_arrayIndex, i_cubeFace);
	ubyte pixelSize = mLocalFormat.getSize();
	for (size_t i = 0; i < mDimen.compProduct(); i++) {
		memcpy(data + (i * pixelSize), i_color, pixelSize);
	}
}

void Texture::_bind(texture_slot_t i_slot) {
	glActiveTexture(GL_TEXTURE0 + i_slot);
	glBindTexture(mType.toGL(), mId);
}

size_t Texture::getDataSize() const {
	return (size_t)mDimen.compProduct() * (size_t)mLocalFormat.getSize();
}

void Texture::pushData() {
	GLenum textureType = mType.toGL();
	GLint gpuFormat = mGPUFormat.toGL();
	GLenum localFormat = mLocalFormat.toGLSimple();
	GLenum localType = mLocalFormat.getPushableType().toGL();
	uint nMipmaps = (uint)std::log2(*std::max_element(mDimen.pointer(), mDimen.pointer() + mType.getBasicDimen())) + 1;

	if (!mId) {
		glCreateTextures(textureType, 1, &mId);
		glBindTexture(textureType, mId);
		switch (mType.mValue) {
		case TextureType::Value::single_1: 
			glTexStorage1D(textureType, nMipmaps, gpuFormat, mDimen.x()); 
			break;
		case TextureType::Value::array_1: 
			glTexStorage2D(textureType, nMipmaps, gpuFormat, mDimen.x(), mDimen.z()); 
			break;
		case TextureType::Value::single_2: 
			glTexStorage2D(textureType, nMipmaps, gpuFormat, mDimen.x(), mDimen.y()); 
			break;
		case TextureType::Value::array_2: 
			glTexStorage3D(textureType, nMipmaps, gpuFormat, mDimen.x(), mDimen.y(), mDimen.z()); 
			break;
		case TextureType::Value::single_msaa_2: 
			glTexStorage2DMultisample(textureType, nMipmaps, gpuFormat, mDimen.x(), mDimen.y(), mType.mMSAA); 
			break;
		case TextureType::Value::array_msaa_2: 
			glTexStorage3DMultisample(textureType, nMipmaps, gpuFormat, mDimen.x(), mDimen.y(), mDimen.z(), mType.mMSAA); 
			break;
		case TextureType::Value::single_cube_2:
			for (ubyte i = 0; i < 6; i++) {
				glTexStorage2D(CubeMapFace::fromIndex(i), nMipmaps, gpuFormat, mDimen.x(), mDimen.y());
			}
			break;
		case TextureType::Value::array_cube_2:
			for (ubyte i = 0; i < 6; i++) {
				glTexStorage3D(CubeMapFace::fromIndex(i), nMipmaps, gpuFormat, mDimen.x(), mDimen.y(), mDimen.z());
			}
			break;
		case TextureType::Value::single_3: 
			glTexStorage3D(textureType, nMipmaps, gpuFormat, mDimen.x(), mDimen.y(), mDimen.z()); 
			break;
		default: fail();
		}
	}

	glBindTexture(textureType, mId);
	switch (mType.mValue) {
	case TextureType::Value::single_1:
		glTexSubImage1D(textureType, 0, 0, mDimen.x(), localFormat, localType, mData);
		break;
	case TextureType::Value::array_1:
		glTexSubImage2D(textureType, 0, 0, 0, mDimen.x(), mDimen.z(), localFormat, localType, mData);
		break;
	case TextureType::Value::single_2:
	case TextureType::Value::single_msaa_2:
		glTexSubImage2D(textureType, 0, 0, 0, mDimen.x(), mDimen.y(), localFormat, localType, mData);
		break;
	case TextureType::Value::array_2:
	case TextureType::Value::array_msaa_2:
		glTexSubImage3D(textureType, 0, 0, 0, 0, mDimen.x(), mDimen.y(), mDimen.z(), localFormat, localType, mData);
		break;
	case TextureType::Value::single_cube_2:
		for (ubyte i = 0; i < 6; i++) {
			CubeMapFace face = CubeMapFace::fromIndex(i);
			glTexSubImage2D(face.toGL(), 0, 0, 0, mDimen.x(), mDimen.y(), localFormat, localType, getData(0, face));
		}
		break;
	case TextureType::Value::array_cube_2:
		for (ubyte i = 0; i < 6; i++) {
			CubeMapFace face = CubeMapFace::fromIndex(i);
			glTexSubImage3D(face.toGL(), 0, 0, 0, 0, mDimen.x(), mDimen.y(), mDimen.z(), localFormat, localType, getData(0, face));
		}
		break;
	case TextureType::Value::single_3:
		glTexSubImage3D(textureType, 0, 0, 0, 0, mDimen.x(), mDimen.y(), mDimen.z(), localFormat, localType, mData);
		break;
	default: fail();
	}
	glBindTexture(textureType, 0);
}

void Texture::pullData() {
	GLenum textureType = mType.toGL();

	if (mData == nullptr) {
		mData = new ubyte[getDataSize()];
	}

	glBindTexture(textureType, mId);
	glGetTexImage(textureType, 0, mLocalFormat.toGLSimple(), mLocalFormat.getPushableType().toGL(), mData);
	glBindTexture(textureType, 0);
}

Texture::Settings& Texture::getSettings() {
	return mSettings;
}

void Texture::pushSettings() {
	if (!mId) fail();
	GLenum textureType = mType.toGL();

	glBindTexture(textureType, mId);
	{
		glTexParameteri(textureType, GL_TEXTURE_SWIZZLE_R, mSettings.mSwizzle.r.toGL());
		glTexParameteri(textureType, GL_TEXTURE_SWIZZLE_G, mSettings.mSwizzle.g.toGL());
		glTexParameteri(textureType, GL_TEXTURE_SWIZZLE_B, mSettings.mSwizzle.b.toGL());
		glTexParameteri(textureType, GL_TEXTURE_SWIZZLE_A, mSettings.mSwizzle.a.toGL());

		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, mSettings.mEdgeMode.s.toGL());
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, mSettings.mEdgeMode.t.toGL());
		glTexParameteri(textureType, GL_TEXTURE_WRAP_R, mSettings.mEdgeMode.r.toGL());

		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, mSettings.mFilter.mMagnify.toGL());
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, mSettings.mFilter.mMinify.toGL());
	}
	glBindTexture(textureType, 0);
}

template<class T>
void Texture::_loadFile(const std::string& i_filename, uint i_arrayIndex, CubeMapFace i_cubeFace) {
	cimg_library::CImg<T> image((i_filename).c_str());
	if (mData == nullptr) {
		mDimen.x() = image.width();
		mDimen.y() = image.height();
		if (mDimen.z() == 0 || mDimen.w() == 0) fail();
		mData = new ubyte[getDataSize()];
	}
	else if (mDimen != Vector4ui(image.width(), image.height(), 0, 0)) {
		fail();
	}

	T* data = (T*)getData(i_arrayIndex, i_cubeFace);
	Vector3ui dimen(mLocalFormat.getDepth(), mDimen.x(), mDimen.y());
	cimg_forXYC(image, x, y, c) {
		data[Vector3ui(c, x, y).getAsCoords(dimen)] = image(x, y, c);
	}
}

template void Texture::_loadFile<sbyte>(const std::string&, uint, CubeMapFace);
template void Texture::_loadFile<ubyte>(const std::string&, uint, CubeMapFace);
template void Texture::_loadFile<short>(const std::string&, uint, CubeMapFace);
template void Texture::_loadFile<ushort>(const std::string&, uint, CubeMapFace);
template void Texture::_loadFile<float>(const std::string&, uint, CubeMapFace);

Texture::Settings::Settings() :
mSwizzle{ 
	Swizzle::Value::channel_1, 
	Swizzle::Value::channel_2, 
	Swizzle::Value::channel_3, 
	Swizzle::Value::channel_4 
},
mEdgeMode{ 
	EdgeBehavior::Value::clamp, 
	EdgeBehavior::Value::clamp, 
	EdgeBehavior::Value::clamp 
},
mFilter{ 
	TextureFilter(
		TextureFilter::Value::nearest, 
		TextureFilter::Value::nearest
	), 
	TextureFilter(
		TextureFilter::Value::nearest, 
		TextureFilter::Value::nearest
	) 
}
{}
