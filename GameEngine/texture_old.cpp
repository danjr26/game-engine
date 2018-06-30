#include "texture.h"
#include "framework.h"

/*
Texture2::Texture2(std::string path, std::string name, GLenum intformat, Flags flags) :
Resource(path, name),
data		(nullptr),
vertexArrayID		(0),
pixformat	(0),
intformat	(intformat),
w			(0),
h			(0),
flags		(flags),
loaded		(false) {
	glGenTextures(1, &vertexArrayID);
}

Texture2::~Texture2() {
	if(data != nullptr)
		delete[] data;
	if(vertexArrayID)
		glDeleteTextures(1, &vertexArrayID);
	Deactivate	();
}

void Texture2::Load_TGA() {
	if (loaded == true)
		Die("Texture2.Load_TGA: Texture object cannot be reused");
	loaded = true;

	std::ifstream	file;
	int				filesize;
	bool			rle;

	file.open(path + name, ios_base::binary);

	if (!file.is_open())
		Die(std::string("Texture2.Load_TGA: *.tga file '") + path + name + std::string("' not found."));

	file.seekg(0, file.end);
	filesize = (int)file.tellg();
	file.seekg(0, file.beg);

	if (filesize < 18)
		Die("Texture2.Load_TGA: empty file.");

	uchar* buffer = new uchar[filesize];
	file.read((char*)buffer, filesize);

	file.close();

	rle =	(buffer[2] & 0x08);
	w =		(int)buffer[12] + ((int)buffer[13] << 8);
	h =		(int)buffer[14] + ((int)buffer[15] << 8);
	ncomp =	buffer[16] / 8;

	int off = 18 + buffer[0];
	data = new uchar[w * h * ncomp];
	if (rle) {
		int i = 0;
		int j = 0;
		int n = 0;
		while (i < w * h * ncomp) {
			n = buffer[off + j] % 0x80 + 1;
			if (buffer[off + j] & 0x80) {
				for (int k = 0; k<n; k++) {
					for (int b = 0; b<ncomp; b++) {
						data[i] = buffer[off + j + 1 + b];
						i++;
					}
				}
				j += 1 + ncomp;
			}
			else {
				for (int k = 0; k<n; k++) {
					for (int b = 0; b<ncomp; b++) {
						data[i] = buffer[off + j + 1 + k * ncomp + b];
						i++;
					}
				}
				j += 1 + n * ncomp;
			}
		}
	}
	else {
		memcpy(data, &buffer[off], w * h * ncomp);
	}

	pixformat = Calc_Pixel_Format(intformat, true);

	Load_To_OpenGL();

	delete[] buffer;

	if (!(flags & TextureFlags::readable)) {
		delete[] data;
		data = nullptr;
	}
}

void Texture2::Load_Raw(uchar* data, uint w, uint h, GLenum pixformat) {
	if (loaded == true)
		Die("Texture2.Load_TGA: Texture object cannot be reused");
	loaded = true;

	this->pixformat = pixformat;
	this->w = w;
	this->h = h;

	ncomp = Calc_Components(pixformat);

	this->data = new uchar[w * h * ncomp];
	memcpy(this->data, data, w * h * ncomp);

	Load_To_OpenGL();

	if (!(flags & TextureFlags::readable)) {
		delete[] this->data;
		this->data = nullptr;
	}
}

void Texture2::Load_Blank(uchar* color, uint w, uint h, int ncomp) {
	if (loaded == true)
		Die("Texture2.Load_TGA: Texture object cannot be reused");
	loaded = true;

	this->w = w;
	this->h = h;
	this->ncomp = ncomp;

	pixformat = Calc_Pixel_Format(intformat, false);

	data = new uchar[w * h * ncomp];
	for (int i = 0; i < w * h; i++) {
		for (int j = 0; j < ncomp; j++) {
			data[i * ncomp + j] = color[j];
		}
	}

	Load_To_OpenGL();
	
	if (!(flags & TextureFlags::readable)) {
		delete[] data;
		data = nullptr;
	}
}

void Texture2::Load_To_OpenGL() {
	glBindTexture(GL_TEXTURE_2D, vertexArrayID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLenum filter =
		(flags & TextureFlags::nearestlinearbit == TextureFlags::nearest) ?
		GL_NEAREST :
		GL_LINEAR;
	GLenum wrap =
		(flags & TextureFlags::clamprepeatbit == TextureFlags::clamp) ?
		GL_CLAMP :
		GL_REPEAT;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	Swizzle();

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		intformat,
		w,
		h,
		0,
		pixformat,
		GL_UNSIGNED_BYTE,
		data
		);

	glBindTexture(GL_TEXTURE_2D, 0);
}

uchar* Texture2::Read() {
	if (!(flags & TextureFlags::readable))
		Die("Texture2.Read: texture not formatted for reading");
	return data;
}

void Texture2::Activate() {
	glBindTexture	(GL_TEXTURE_2D, vertexArrayID);
}

void Texture2::Deactivate() {
	glBindTexture	(GL_TEXTURE_2D, 0);
}

/*void Texture2::Swizzle() {
	GLint swizzle[4];
	switch (ncomp) {
	case 2:
		swizzle[0] = GL_RED;
		swizzle[1] = GL_RED;
		swizzle[2] = GL_RED;
		swizzle[3] = GL_GREEN;
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		break;
	case 1:
		swizzle[0] = GL_RED;
		swizzle[1] = GL_RED;
		swizzle[2] = GL_RED;
		swizzle[3] = GL_ONE;
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		break;
	}
}

GLenum Texture2::Calc_Pixel_Format(GLenum intformat, bool bgr) {
	GLenum pixformat;
	switch (intformat) {
	case GL_RGBA:
	case GL_RGBA4:
	case GL_RGBA8:
	case GL_RGBA12:
	case GL_RGBA16:
	case GL_RGBA16F:
	case GL_RGBA32F:
		pixformat = (bgr) ? GL_BGRA : GL_RGBA;
		break;
	case GL_RGB:
	case GL_RGB4:
	case GL_RGB8:
	case GL_RGB12:
	case GL_RGB16:
	case GL_RGB16F:
	case GL_RGB32F:
		pixformat = (bgr) ? GL_BGR : GL_RGB;
		break;
	case GL_RG:
	case GL_RG8:
	case GL_RG16:
	case GL_RG16F:
	case GL_RG32F:
		pixformat = GL_RG;
		break;
	case GL_R:
	case GL_R8:
	case GL_R16:
	case GL_R16F:
	case GL_R32F:
		pixformat = GL_R;
		break;
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_COMPONENT16:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT32F:
		pixformat = GL_DEPTH_COMPONENT;
		break;
	case GL_STENCIL_INDEX:
	case GL_STENCIL_INDEX1:
	case GL_STENCIL_INDEX4:
	case GL_STENCIL_INDEX8:
	case GL_STENCIL_INDEX16:
		pixformat = GL_STENCIL_INDEX;
		break;
	default:
		Die("Texture2.Calc_Pixel_Format: invalid internal format");
	}
	return pixformat;
}

int Texture2::Calc_Components(GLenum pixformat) {
	int ncomp;
	switch (pixformat) {
	case GL_RGBA:
	case GL_BGRA:
		ncomp = 4;
		break;
	case GL_RGB:
	case GL_BGR:
		ncomp = 3;
		break;
	case GL_RG:
		ncomp = 2;
		break;
	case GL_RED:
		ncomp = 1;
		break;
	default:
		Die("Texture2.Pixel_Format_To_Components: invalid pixformat");
	}
	return ncomp;
}

uint Texture2::W() {
	return w;
}

uint Texture2::H() {
	return h;
}

*/