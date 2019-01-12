#include "shader.h"
#include "game_engine.h"

Shader::Shader(Shader::Type in_type, std::string in_filename) :
type(in_type) {
	std::fstream file;
	file.open(in_filename);

	if (!file.is_open()) {
		Log::main(std::string("error: could not open file '") + in_filename + "'");
		GE.Exit();
	}

	file.seekg(0, std::ios::end);
	int length = file.tellg();   
	char* buffer = new char[length]; 

	for (int i = 0; i < length; i++) {
		buffer[i] = '\0';
	}

	file.seekg(0, std::ios::beg); 
	file.read(buffer, length);

	file.close();

	id = glCreateShader((GLuint)in_type);
	glShaderSource(id, 1, &buffer, &length);
	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint logLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		glGetShaderInfoLog(id, logLength, nullptr, log);
		Log::main(std::string("\nerror: shader '") + in_filename + "' compilation failed:\n" + log);
		delete[] log;
		GE.Exit();
	}

	delete[] buffer;
}

Shader::~Shader() {
	glDeleteShader(id);
}

Shader::Type Shader::Get_Type() {
	return type;
}

Shader::Type Shader::Parse_Type(const std::string& in_text) {
	if (in_text == "vertex") return Type::vertex;
	if (in_text == "fragment") return Type::fragment;
	if (in_text == "geometry") return Type::geometry;
	if (in_text == "tess_control") return Type::tess_control;
	if (in_text == "tess_eval") return Type::tess_eval;
	if (in_text == "compute") return Type::compute;
	Log::main(std::string("invalid shader type string literal '") + in_text + "'");
	GE.Exit();
}

