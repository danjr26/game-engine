#include "../include/internal/shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() :
mId(0),
mType()
{}

Shader::~Shader() {
	destroy();
}

void Shader::init(ShaderType i_type, const std::string* i_texts, uint i_nTexts) {
	if (isValid()) fail();
	mId = glCreateShader(mType.toGL());
	std::vector<const char*> texts;
	for (uint i = 0; i < i_nTexts; i++) {
		texts.push_back(i_texts->c_str());
	}
	glShaderSource(mId, i_nTexts, texts.data(), nullptr);
	glCompileShader(mId);

	GLint success = 0;
	glGetShaderiv(mId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logLength = 0;
		glGetShaderiv(mId, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		glGetShaderInfoLog(mId, logLength, nullptr, log);
		std::cout << log << std::endl;
		delete[] log;
		fail();
	}
}

void Shader::initFromFile(ShaderType i_type, const std::string& i_filename) {
	std::string text;
	fileToString(i_filename, text);
	init(i_type, &text, 1);
}

void Shader::destroy() {
	if (!isValid()) return;
	glDeleteShader(mId);
	mId = 0;
}

bool Shader::isValid() const {
	return mId;
}

ShaderType Shader::getType() const {
	return mType;
}
