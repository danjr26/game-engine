#ifndef TEXTURE_H
#define TEXTURE_H

#include <Windows.h>
#include <gl/glew.h>
#include <GL/wglew.h>
#include <fstream>
#include <string>
#include "resources.h"



namespace TextureFlags {
	enum {
		readable =	0x01,
		mipmaps =	0x02
	};
}

class Texture2D: public Resource {
	friend class Framebuffer;
private:
	CustomArray
		<uchar> data;
	uint	textureID;

	GLenum	pixelFormat;
	GLenum	internalFormat;

	Vector2i dimensions;
	uchar	nComponents;
	Flags	flags;

	bool	loaded;

public:
			Texture2D		(string in_path, string in_name, GLenum in_internalFormat, Flags in_flags = TextureFlags::mipmaps);
			Texture2D		(string in_name, Vector2i in_dimensions, CustomArray<uchar> in_data, uchar in_nComponents = 4);
			Texture2D		(string in_name, Vector2i in_dimensions, Color4f in_color = Color4f(1.0, 1.0, 1.0, 1.0), uchar in_nComponents = 4);
			~Texture2D		();

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
	Texture2D* texture;
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

class Framebuffer {
public:
	static Framebuffer* active;

private:
	uint		vertexArrayID;

	Texture2D*	color;
	Texture2D*	depth;
	Texture2D*	stencil;

	uint		w, h;

	struct {
		uint vertexArrayID, positionBuffer, uvBuffer;
	} rect;

public:
			Framebuffer	(Texture2D* color, Texture2D* depth, Texture2D* stencil);
			~Framebuffer();
	void	Apply		(Flags mode);
	void	Unapply		(Flags mode);
	void	Copy_To		(Framebuffer* fb, Flags buffers);
	void	Render_To	(Framebuffer* fb);

	static void Apply_Default(Flags mode);
};

#endif
