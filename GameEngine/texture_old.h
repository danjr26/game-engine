#ifndef TEXTURE_H
#define TEXTURE_H

#include <Windows.h>
#include <gl/glew.h>
#include <GL/wglew.h>
#include <fstream>
#include <string>
#include <vector>
#include "vector.h"
#include "definitions.h"


namespace TextureFlags {
	enum {
		readable =	0x01,
		mipmaps =	0x02
	};
}

class Texture2 {
	friend class Framebuffer;

public:
	enum class Flags : uint {
		readable =	0x01,
		mipmaps =	0x02
	};

private:
	std::vector<uchar> data;
	GLuint id;

	GLenum pixelFormat;
	GLenum internalFormat;

	Vector2i dimensions;
	uchar nComponents;
	Flags flags;

	bool isLoaded;

public:
			Texture2		(string in_path, string in_name, GLenum in_internalFormat, Flags in_flags = TextureFlags::mipmaps);
			Texture2		(string in_name, Vector2i in_dimensions, CustomArray<uchar> in_data, uchar in_nComponents = 4);
			Texture2		(string in_name, Vector2i in_dimensions, Color4f in_color = Color4f(1.0, 1.0, 1.0, 1.0), uchar in_nComponents = 4);
			~Texture2		();

	void	Load_TGA		();

	uchar*	Read			();

	void 	Activate		();	
	void 	Deactivate		();

	uint	W				();
	uint	H				();

private:
	void	Load_To_OpenGL	();
//	void	Swizzle			();

	static GLenum	Calc_Pixel_Format	(GLenum intformat);
	static int		Calc_Components		(GLenum pixformat);
};

namespace TextureInfluences {
	enum {
		ambient_color,
		diffuse_color,
		specular_color,
		specular_hardness,
		emission_color,
		color_intensity,
		alpha_intensity,
		number_influences
	};
}

enum TextureInfluence {
	ambient,
	diffuse,
	specular,
	hardness,
	emission,
	intensity,
	alpha,
	normal,
	num_textures_influences
};

namespace TextureFiltering {
	enum {
		none,
		bilinear,
		trilinear
	};
}

class TextureInstance2D {
	Texture2* texture;
	CustomArray
		<float> influences;
	uint shaderLocation;
	
};

namespace FramebufferFlags {
	enum : Flags {
		color =		0x01,
		depth =		0x02,
		stencil =	0x04,

		read =		0x01,
		write =		0x02,
		readwrite = read | write
	};
}

#endif
