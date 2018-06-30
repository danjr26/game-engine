#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <string>
#include <fstream>
#include "log.h"
#include "definitions.h"
#include "asset.h"

class Shader : public Asset {
	friend class ShaderProgram;
public:
	enum AttributeIndex {
		position = 0,
		normal = 1,
		color = 2,
		uv = 3
	};

	enum class Type : GLuint {
		vertex = GL_VERTEX_SHADER,
		fragment = GL_FRAGMENT_SHADER,
		geometry = GL_GEOMETRY_SHADER,
		tess_control = GL_TESS_CONTROL_SHADER,
		tess_eval = GL_TESS_EVALUATION_SHADER,
		compute = GL_COMPUTE_SHADER
	};

private:
	Type type;
	GLuint id;

public:
	Shader(Type in_type, std::string in_filename);
	~Shader();
	Type Get_Type();
};



#endif