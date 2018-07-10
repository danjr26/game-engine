#ifndef TEXTURE_H
#define TEXTURE_H

#include "definitions.h"
#include <GL\glew.h>
#include "definitions.h"
#include "vector.h"
#define cimg_use_png
#include "CImg-2.2.3\CImg.h"
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
	Type type;

	ubyte* data;
	GLuint id;

	GLenum pixelFormat;
	GLenum internalFormat;
	GLenum target;

	Vector3i dimensions;
	ubyte nChannels;
	ubyte nBitsPerChannel;
	ubyte nSamplesMSAA;

	uint flags;

	TextureSettings activeSettings;

public:
	Texture(Type in_type, std::string in_filename, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA = 0);
	Texture(Type in_type, Vector3i in_dimensions, ubyte* in_data, ubyte in_nChannels, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA = 0);
	Texture(Type in_type, Vector3i in_dimensions, ColorRGBAc in_color, ubyte in_nChannels, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA = 0);
	Texture(Type in_type, Vector3i in_dimensions, ubyte in_nChannels, ubyte in_nBitsPerChannel, Flags in_flags, ubyte in_nSamplesMSAA = 0);

	const ubyte* Read();
	void Refresh_From_OpenGL();

	void Use(ubyte in_slot = 0);
	void Use_None(ubyte in_slot = 0);

	Type Get_Type();
	Vector3i Get_Dimensions();
	ubyte Get_Number_Channels();
	ubyte Get_Number_Bits_Per_Channel();
	ubyte Get_Number_Samples_MSAA();
	uint Get_Flags();
	TextureSettings Get_Active_Settings();

protected:
	void Finish_Setup();
	void Load_To_OpenGL();

	GLenum Determine_Pixel_Format();
	GLenum Determine_Internal_Format();
	GLenum Determine_Target();
	ubyte Determine_Span();
};

#endif