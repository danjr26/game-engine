#ifndef TEXTURE_H
#define TEXTURE_H

#include "definitions.h"
#include <GL\glew.h>
#include "definitions.h"
#include "vector.h"
#define png_debug
#define cimg_use_png
#include "CImg.h"
#include "texture_settings.h"
#include "asset.h"

class Texture : public Asset {
	friend class TextureSettings;
	friend class Framebuffer;
public:
	enum Flags : uint {
		none = 0,
		readable = 1,
		mipmaps = readable << 1,
		hdr = mipmaps << 1,
		depth = hdr << 1
	};

	enum Type : GLenum {
		_1d = GL_TEXTURE_1D,
		_1d_array = GL_TEXTURE_1D_ARRAY,
		_2d = GL_TEXTURE_2D,
		_2d_array = GL_TEXTURE_2D_ARRAY,
		_3d = GL_TEXTURE_3D,
		cubemap_positive_x = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		cubemap_positive_y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		cubemap_positive_z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		cubemap_negative_x = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		cubemap_negative_y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		cubemap_negative_z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

private:
	Type mType;

	ubyte* mData;
	GLuint mID;

	GLenum mPixelFormat;
	GLenum mInternalFormat;
	GLenum mTarget;

	Vector3ui mDimensions;
	ubyte mNChannels;
	ubyte mNBitsPerChannel;
	ubyte mNSamplesMSAA;

	uint mFlags;

	TextureSettings mActiveSettings;

public:
	Texture(Type in_type, std::string in_filename, ubyte in_nBitsPerChannel, uint in_flags, ubyte in_nSamplesMSAA = 0);
	Texture(Type in_type, Vector3ui in_dimensions, ubyte* in_data, ubyte in_nChannels, ubyte in_nBitsPerChannel, uint in_flags, ubyte in_nSamplesMSAA = 0);
	Texture(Type in_type, Vector3ui in_dimensions, ColorRGBAc in_color, ubyte in_nChannels, ubyte in_nBitsPerChannel, uint in_flags, ubyte in_nSamplesMSAA = 0);
	Texture(Type in_type, Vector3ui in_dimensions, ubyte in_nChannels, ubyte in_nBitsPerChannel, uint in_flags, ubyte in_nSamplesMSAA = 0);

	~Texture();

	const ubyte* read();
	void refreshFromOpenGL();

	void use(ubyte in_slot = 0);
	void useNone(ubyte in_slot = 0);

	Type getType();
	Vector3ui getDimensions();
	ubyte getNumberChannels();
	ubyte getNumberBitsPerChannel();
	ubyte getNumberSamplesMSAA();
	uint getFlags();
	const TextureSettings& getActiveSettings();

	static void loadXMLList(std::string in_filename);

protected:
	void finishSetup();
	void loadToOpenGL();

	GLenum determinePixelFormat();
	GLenum determineInternalFormat();
	GLenum determineTarget();
	ubyte determineSpan();
};

#endif