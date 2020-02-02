#include "../include/internal/shader_program.h"
#include <iostream>

ShaderProgram::ShaderProgram() :
mId(0)
{}

ShaderProgram::~ShaderProgram() {
	destroy();
}

void ShaderProgram::init(const Shader* const* i_shaders, uint i_nShaders) {
	if (isValid()) fail();

	mId = glCreateProgram();
	for (uint i = 0; i < i_nShaders; i++) {
		glAttachShader(mId, i_shaders[i]->mId);
	}
	glLinkProgram(mId);

	GLint success = 0;
	glGetProgramiv(mId, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logLength = 0;
		glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		glGetProgramInfoLog(mId, logLength, nullptr, log);
		std::cout << log << std::endl;
		delete[] log;
		fail();
	}
}

void ShaderProgram::destroy() {
	if (!isValid()) return;
	glDeleteProgram(mId);
	mId = 0;
}

bool ShaderProgram::isValid() const {
	return mId;
}

void ShaderProgram::_activate() {
	glUseProgram(mId);
}
