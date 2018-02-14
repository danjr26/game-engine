#include "texture.h"
#include "framework.h"

Texture2D::Texture2D(std::string path, std::string name, GLenum intformat, Flags flags) :
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

Texture2D::~Texture2D() {
	if(data != nullptr)
		delete[] data;
	if(vertexArrayID)
		glDeleteTextures(1, &vertexArrayID);
	Deactivate	();
}

void Texture2D::Load_TGA() {
	if (loaded == true)
		Die("Texture2D.Load_TGA: Texture object cannot be reused");
	loaded = true;

	std::ifstream	file;
	int				filesize;
	bool			rle;

	file.open(path + name, ios_base::binary);

	if (!file.is_open())
		Die(std::string("Texture2D.Load_TGA: *.tga file '") + path + name + std::string("' not found."));

	file.seekg(0, file.end);
	filesize = (int)file.tellg();
	file.seekg(0, file.beg);

	if (filesize < 18)
		Die("Texture2D.Load_TGA: empty file.");

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

void Texture2D::Load_Raw(uchar* data, uint w, uint h, GLenum pixformat) {
	if (loaded == true)
		Die("Texture2D.Load_TGA: Texture object cannot be reused");
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

void Texture2D::Load_Blank(uchar* color, uint w, uint h, int ncomp) {
	if (loaded == true)
		Die("Texture2D.Load_TGA: Texture object cannot be reused");
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

void Texture2D::Load_To_OpenGL() {
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

uchar* Texture2D::Read() {
	if (!(flags & TextureFlags::readable))
		Die("Texture2D.Read: texture not formatted for reading");
	return data;
}

void Texture2D::Activate() {
	glBindTexture	(GL_TEXTURE_2D, vertexArrayID);
}

void Texture2D::Deactivate() {
	glBindTexture	(GL_TEXTURE_2D, 0);
}

/*void Texture2D::Swizzle() {
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
}*/		

GLenum Texture2D::Calc_Pixel_Format(GLenum intformat, bool bgr) {
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
		Die("Texture2D.Calc_Pixel_Format: invalid internal format");
	}
	return pixformat;
}

int Texture2D::Calc_Components(GLenum pixformat) {
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
		Die("Texture2D.Pixel_Format_To_Components: invalid pixformat");
	}
	return ncomp;
}

uint Texture2D::W() {
	return w;
}

uint Texture2D::H() {
	return h;
}

Framebuffer* Framebuffer::active = nullptr;

Framebuffer::Framebuffer(Texture2D* color, Texture2D* depth, Texture2D* stencil) :
color	(color),
depth	(depth),
stencil	(stencil),
w		(0xffffffff),
h		(0xffffffff) {
	glGenFramebuffers(1, &vertexArrayID);
	glBindFramebuffer(GL_FRAMEBUFFER, vertexArrayID);

	if (color != nullptr) {
		if (!color->loaded)
			Die("Framebuffer.Framebuffer: texture not loaded");
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color->vertexArrayID, 0);
		w = min(w, color->w);
		h = min(h, color->h);
	}

	if (depth != nullptr) {
		if (!depth->loaded)
			Die("Framebuffer.Framebuffer: texture not loaded");
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth->vertexArrayID, 0);
		w = min(w, depth->w);
		h = min(h, depth->h);
	}

	if (stencil != nullptr) {
		if (!stencil->loaded)
			Die("Framebuffer.Framebuffer: texture not loaded");
		glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, stencil->vertexArrayID, 0);
		w = min(w, stencil->w);
		h = min(h, stencil->h);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Die("Framebuffer.Framebuffer: error initializing framebuffer");

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glGenVertexArrays(1, &rect.vertexArrayID);
	glBindVertexArray(rect.vertexArrayID);
	glGenBuffers(1, &rect.positionBuffer);
	glGenBuffers(1, &rect.uvBuffer);

	Vector3f zeros[] = {
		Vector3f(),
		Vector3f(),
		Vector3f(),
		Vector3f()
	};

	glBindBuffer(GL_ARRAY_BUFFER, rect.positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 4, zeros, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, rect.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Framebuffer::~Framebuffer() {
	if (color != nullptr)
		delete color;
	if (depth != nullptr)
		delete depth;
	if (stencil != nullptr)
		delete stencil;
	glDeleteFramebuffers(1, &vertexArrayID);

	glDeleteBuffers(2, &rect.positionBuffer);
	glDeleteVertexArrays(1, &rect.vertexArrayID);
}

void Framebuffer::Apply(Flags mode) {
	switch (mode) {
	case FramebufferFlags::write:
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, vertexArrayID);
		glViewport(0, 0, w, h);
		break;
	case FramebufferFlags::read:
		glBindFramebuffer(GL_READ_FRAMEBUFFER, vertexArrayID);
		break;
	case FramebufferFlags::readwrite:
		glBindFramebuffer(GL_FRAMEBUFFER, vertexArrayID);
		glViewport(0, 0, w, h);
		break;
	default:
		Die("Framebuffer.Apply: invalid flags");
	}
	
}

void Framebuffer::Unapply(Flags mode) {
	Apply_Default(mode);
}

void Framebuffer::Copy_To(Framebuffer* fb, Flags buffers) {
	Apply(FramebufferFlags::read);
	if (fb == nullptr)
		Unapply(FramebufferFlags::write);
	else
		fb->Apply(FramebufferFlags::write);

	GLbitfield mask = 0;
	if (buffers & FramebufferFlags::color)
		mask |= GL_COLOR_BUFFER_BIT;
	if (buffers & FramebufferFlags::depth)
		mask |= GL_DEPTH_BUFFER_BIT;
	if (buffers & FramebufferFlags::stencil)
		mask |= GL_STENCIL_BUFFER_BIT;

	glBlitFramebuffer(0, 0, w, h, 0, 0, fb->w, fb->h, mask, GL_NEAREST);
	Unapply(FramebufferFlags::readwrite);
}

void Framebuffer::Render_To(Framebuffer* fb) {
	float w, h;

	Apply(FramebufferFlags::read);
	if (fb == nullptr) {
		Unapply(FramebufferFlags::write);
		w = 800;
		h = 600;
	}
	else {
		fb->Apply(FramebufferFlags::write);
		w = fb->w;
		h = fb->h;
	}

	glDisable(GL_BLEND);
	glDepthMask(0);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	/*
	Vector3f positions[4] = {
		Vector3f(0, h, 0),
		Vector3f(0, 0, 0),
		Vector3f(w, h, 0),
		Vector3f(w, 0, 0)
	};*/
	
	Vector3f positions[4] = {
		Vector3f(-1, -1, 0),
		Vector3f(-1, 1, 0),
		Vector3f(1, 1, 0),
		Vector3f(1, -1, 0)
	};

	Vector2f uvBuffer[4] = {
		Vector2f(0, 0),
		Vector2f(0, 1),
		Vector2f(1, 1),
		Vector2f(1, 0)
	};

	glBindVertexArray(rect.vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, rect.positionBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector3f) * 4, positions);

	glBindBuffer(GL_ARRAY_BUFFER, rect.uvBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector2f) * 4, uvBuffer);

	ShaderProgram::Activate("framebuffer_shader");

	color->Activate();
	uint coltexloc = glGetUniformLocation(ShaderProgram::active->vertexArrayID, "tex");
	glUniform1i(coltexloc, 0);

	glDrawArrays(GL_QUADS, 0, 4);

	color->Deactivate();

	ShaderProgram::Deactivate();

	glDepthMask(1);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	Unapply(FramebufferFlags::readwrite);
}

void Framebuffer::Apply_Default(Flags mode) {
	switch (mode) {
	case FramebufferFlags::write:
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0, 0, 800, 600);
		break;
	case FramebufferFlags::read:
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		break;
	case FramebufferFlags::readwrite:
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 800, 600);
		break;
	default:
		Die("Framebuffer.Unapply: invalid flags");
	}
}

