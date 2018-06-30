#include "framebuffer.h"


/*
Framebuffer* Framebuffer::active = nullptr;

Framebuffer::Framebuffer(Texture2* color, Texture2* depth, Texture2* stencil) :
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
	};

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

*/

Framebuffer::Framebuffer(std::vector<Texture*> in_colorTextures, Texture* in_depthTexture) :
colorTextures(in_colorTextures),
depthTexture(in_depthTexture) {
	if (colorTextures.size() == 0) {
		Log::main("error: no color textures passed to framebuffer");
		exit(-1);
	}

	Vector3i dimensions = colorTextures[0]->Get_Dimensions();
	for (uint i = 1; i < colorTextures.size(); i++) {
		if (colorTextures[i] == nullptr || colorTextures[i]->Get_Dimensions() != dimensions) {
			Log::main("error: no framebuffer texture mismatch");
			exit(-1);
		}
	}

	if(depthTexture != nullptr && depthTexture->Get_Dimensions() != dimensions) {
		Log::main("error: no framebuffer texture mismatch");
		exit(-1);
	}

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	for (uint i = 0; i < colorTextures.size(); i++) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, colorTextures[i]->id, 0);
	}

	if (depthTexture != nullptr) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->id, 0);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::main("error: cannot initialize framebuffer");
		exit(-1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &id);
}

std::vector<Texture*> Framebuffer::Get_Color_Textures() {
	return colorTextures;
}

Texture* Framebuffer::Get_Color_Texture(uint in_index) {
	return colorTextures[in_index];
}

Texture* Framebuffer::Get_Depth_Texture() {
	return depthTexture;
}

Vector2i Framebuffer::Get_Dimensions() const {
	return (Vector2i)colorTextures[0]->Get_Dimensions();
}

void Framebuffer::_Draw_To_This() {
	Vector3i dimensions = colorTextures[0]->Get_Dimensions();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
	glViewport(0, 0, dimensions.X(), dimensions.Y());
}


