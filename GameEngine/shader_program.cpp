#include "shader_program.h"

ShaderProgram::ShaderProgram(std::vector<Shader*> in_shaders) {
	if (in_shaders.size() == 0) {
		Log::main("error: no shaders provided to program linker");
	}

	id = glCreateProgram();
	
	for (uint i = 0; i < in_shaders.size(); i++) {
		glAttachShader(id, in_shaders[i]->id);
	}

	glLinkProgram(id);

	GLint success = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		glGetProgramInfoLog(id, logLength, nullptr, log);
		Log::main(std::string("\nerror: shader program linking failed:\n") + log);
	}
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id);
}

void ShaderProgram::Use() {
	glUseProgram(id);
}

GLint ShaderProgram::Get_Uniform_Location(std::string in_name) {
	return glGetUniformLocation(id, in_name.c_str());
}

void ShaderProgram::Use_None() {
	glUseProgram(0);
}
