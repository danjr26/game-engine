#include "skybox.h"
#include "framework.h"

Skybox::Skybox(string path, string name) :
RenderComponent(RenderType::first) {
	Texture2* texes[6];
	texes[0] = new Texture2(path, name + "_right1.tga", GL_RGBA8, TextureFlags::readable);
	texes[1] = new Texture2(path, name + "_left2.tga", GL_RGBA8, TextureFlags::readable);
	texes[2] = new Texture2(path, name + "_top3.tga", GL_RGBA8, TextureFlags::readable);
	texes[3] = new Texture2(path, name + "_bottom4.tga", GL_RGBA8, TextureFlags::readable);
	texes[4] = new Texture2(path, name + "_front5.tga", GL_RGBA8, TextureFlags::readable);
	texes[5] = new Texture2(path, name + "_back6.tga", GL_RGBA8, TextureFlags::readable);
	for (int i = 0; i < 6; i++)
		texes[i]->Load_TGA();

	Color4c white = Color4c(255, 255, 255, 255);
	Texture2 whitetex = Texture2("", "", GL_RGBA8, TextureFlags::readable);
	whitetex.Load_Blank(&white.r, 200, 200, 4);

	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &texglid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texglid);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, texes[0]->W(), texes[0]->H(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texes[0]->Read());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, texes[1]->W(), texes[1]->H(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texes[1]->Read());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, texes[2]->W(), texes[2]->H(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texes[2]->Read());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, texes[3]->W(), texes[3]->H(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texes[3]->Read());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, texes[4]->W(), texes[4]->H(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texes[4]->Read());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, texes[5]->W(), texes[5]->H(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texes[5]->Read());
	

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(GL_TEXTURE0);
	for (int i = 0; i < 6; i++)
		delete texes[i];

	float vertdata[] = {
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, 10.0f, 
		10.0f, 10.0f, 10.0f, 
		10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f, 
		-10.0f, -10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, 10.0f,

		-10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,

		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, -10.0f, 
		-10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,

		10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f, 
		-10.0f, 10.0f, 10.0f, 
		10.0f, 10.0f, 10.0f
	};

	glGenVertexArrays(1, &meshglid);
	glBindVertexArray(meshglid);

	glGenBuffers(1, &vertbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4 * 3, vertdata, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
}

Skybox::~Skybox() {
	glDeleteBuffers(1, &vertbuffer);
	glDeleteTextures(1, &texglid);
	glDeleteVertexArrays(1, &meshglid);
}

void Skybox::Render(Flags callflags) {
	if (callflags == RenderType::first) {
		glDepthMask(0);
		glDisable(GL_CULL_FACE);

		ShaderProgram::Activate("skybox_shader");

		Matrix4f viewmat = viewStack.Top();
		Matrix4f projmat = projectionStack.Top();

		int texloc = glGetUniformLocation(ShaderProgram::active->vertexArrayID, "tex");
		glUniform1i(texloc, 1);

		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "viematrix"), 1, GL_FALSE, viewmat.data);
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgram::active->vertexArrayID, "promatrix"), 1, GL_TRUE, projmat.data);

		glBindVertexArray(meshglid);
		glBindBuffer(GL_ARRAY_BUFFER, vertbuffer);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texglid);

		glDrawArrays(GL_QUADS, 0, 24);

		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		glDepthMask(1);

		ShaderProgram::Deactivate();
	}
}