#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "shader.h"
#include <vector>
#include "asset.h"

class ShaderProgram : public Asset {
private:
	GLuint id;

public:
	ShaderProgram(std::vector<Shader*> in_shaders);
	~ShaderProgram();
	void Use();
	GLint Get_Uniform_Location(std::string in_name);

	static void Use_None();
};

#endif