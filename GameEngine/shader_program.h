#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "shader.h"
#include <vector>
#include "asset.h"

class ShaderProgram : public Asset {
private:
	GLuint mID;

public:
	ShaderProgram(std::vector<Shader*> in_shaders);
	~ShaderProgram();
	void use();
	GLint getUniformLocation(std::string in_name);

	static void useNone();

	static void loadXMLList(std::string in_filename);
};

#endif