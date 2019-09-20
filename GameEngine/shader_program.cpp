#include "shader_program.h"
#include "rapidxml.hpp"
#include <fstream>
#include <sstream>
#include "game_engine.h"

ShaderProgram::ShaderProgram(std::vector<Shader*> in_shaders) {
	if (in_shaders.size() == 0) {
		throw InvalidArgumentException("no shaders provided to linker");
	}

	mID = glCreateProgram();
	
	for (uint i = 0; i < in_shaders.size(); i++) {
		glAttachShader(mID, in_shaders[i]->mID);
	}

	glLinkProgram(mID);

	GLint success = 0;
	glGetProgramiv(mID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logLength = 0;
		glGetProgramiv(mID, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		glGetProgramInfoLog(mID, logLength, nullptr, log);
		throw InvalidArgumentException(std::string("shader linking failed with error message:\n") + log);
	}
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(mID);
}

void ShaderProgram::use() {
	glUseProgram(mID);
}

GLint ShaderProgram::getUniformLocation(std::string in_name) {
	return glGetUniformLocation(mID, in_name.c_str());
}

void ShaderProgram::useNone() {
	glUseProgram(0);
}

void ShaderProgram::loadXMLList(std::string in_filename) {
	std::ifstream file(in_filename);
	if (!file.is_open()) {
		Log::main(std::string("error: cannot open file '") + in_filename + "'");
		GE.quit();
	}

	file.seekg(0, file.end);
	uint length = (uint)file.tellg();
	file.seekg(0, file.beg);

	char* buffer = new char[length];
	for (uint i = 0; i < length; i++) {
		buffer[i] = '\0';
	}
	file.read(buffer, length);
	file.close();

	std::vector<Shader*> shaders;

	rapidxml::xml_document<char> doc;
	doc.parse<0>(buffer);

	auto masterNode = doc.first_node("ShaderList");
	if (!masterNode) {
		Log::main(std::string("error: invalid shader list file '") + in_filename + "'");
		GE.quit();
	}
	
	for (auto node = masterNode->first_node("ShaderProgram"); node; node = node->next_sibling("ShaderProgram")) {
		auto nameAttribute = node->first_attribute("name");

		if (!nameAttribute) {
			Log::main(std::string("error: invalid shader list file '") + in_filename + "'");
			GE.quit();
		}

		std::vector<Shader> myShaders;
		for (auto subNode = node->first_node("Shader"); subNode; subNode = subNode->next_sibling("Shader")) {
			auto typeAttribute = subNode->first_attribute("type");
			auto fileAttribute = subNode->first_attribute("file");

			if (!typeAttribute || !fileAttribute) {
				Log::main(std::string("error: invalid shader list file '") + in_filename + "'");
				GE.quit();
			}

			Shader* shader = new Shader(Shader::parseType(typeAttribute->value()), fileAttribute->value());
			shaders.push_back(shader);
		}

		ShaderProgram* shaderProgram = new ShaderProgram(shaders);
		GE.assets().add(nameAttribute->value(), shaderProgram);


		for (uint i = 0; i < shaders.size(); i++) {
			delete shaders[i];
		}
		shaders.clear();
	}

	delete[] buffer;
}
