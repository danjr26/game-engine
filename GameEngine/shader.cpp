#include "shader.h"
#include "framework.h"

Shader::Shader(GLuint shadertype, string path, string name) :
Resource(path, name),
shadertype(shadertype) {
	fstream file;
	file.open(path + name);
	if (!file.is_open())
		Die(string("Could not find shader file: ") + path + name);
	file.seekg(0, std::ios::end);
	int length = file.tellg();      
	file.seekg(0, std::ios::beg); 
	char* buffer = new char[length]; 
	for (int i = 0; i < length; i++)
		buffer[i] = '\0';
	file.read(buffer, length);
	file.close();

	vertexArrayID = glCreateShader(shadertype);
	glShaderSource(vertexArrayID, 1, &buffer, &length);
	glCompileShader(vertexArrayID);
	GLint success = 0;
	glGetShaderiv(vertexArrayID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logsize = 0;
		glGetShaderiv(vertexArrayID, GL_INFO_LOG_LENGTH, &logsize);
		char* log = new char[logsize];
		glGetShaderInfoLog(vertexArrayID, logsize, nullptr, log);
		Die(name + '\n' + log);
	}
	delete[] buffer;
}

Shader::~Shader() {
	glDeleteShader(vertexArrayID);
}

ShaderProgram* ShaderProgram::active = nullptr;

ShaderProgram::ShaderProgram(string name, Shader* vshader, Shader* fshader, Shader* gshader, Shader* tshader) :
Resource("", name) {
	if (vshader == nullptr || fshader == nullptr)
		Die("Need vertex and fragment shaders!");
	vertexArrayID = glCreateProgram();
	glAttachShader(vertexArrayID, vshader->vertexArrayID);
	glAttachShader(vertexArrayID, fshader->vertexArrayID);
	if(gshader != nullptr)
		glAttachShader(vertexArrayID, gshader->vertexArrayID);
	if(tshader != nullptr)
		glAttachShader(vertexArrayID, tshader->vertexArrayID);
	glLinkProgram(vertexArrayID);

	GLint success = 0;
	glGetProgramiv(vertexArrayID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logsize = 0;
		glGetProgramiv(vertexArrayID, GL_INFO_LOG_LENGTH, &logsize);
		char* log = new char[logsize];
		glGetProgramInfoLog(vertexArrayID, logsize, nullptr, log);
		Die(log);
	}
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(vertexArrayID);
}

void ShaderProgram::Activate() {
	glUseProgram(vertexArrayID);
	active = this;
}

void ShaderProgram::Deactivate() {
	glUseProgram(0);
	active = nullptr;
}

void ShaderProgram::Activate(string name) {
	if (active != nullptr && name == active->name)
		return;
	ShaderProgram* shader = (ShaderProgram*)GEngine::Get().Resource().Get_Resource(name);
	if (shader == nullptr)
		Die("ShaderProgram.Activate: could not find shader '" + name + "'");
	else
		shader->Activate();
}