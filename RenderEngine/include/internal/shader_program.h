#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "shader.h"
#include "uniform_pusher.h"

class ShaderProgram {
friend class RenderEngine;
private:
	GLuint mId;

public:
	ShaderProgram();
	~ShaderProgram();

	void init(const Shader* const* i_shaders, uint i_nShaders);
	void destroy();
	bool isValid() const;

protected:
	void _activate();
};

#endif