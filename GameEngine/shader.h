#ifndef SHADER_H
#define SHADER_H

#include "resources.h"
/*
namespace ShaderFlags {
	enum : Flags {
		resposloc = 0,
		resnorloc = 1,
		rescolloc = 2,
		restxcloc = 3,

		resmodmatloc = 0,
		resviematloc = 1,
		respromatloc = 2,
		rescoltexloc = 3,
		rescoltexbndloc = 4
	};
}*/

struct BlendSettings {
	GLenum	destinationFactor;
	GLenum	sourceFactor;
	GLenum	operation;
	bool	useBlending;

	BlendSettings(bool useBlending = false, GLenum in_destinationFactor = GL_ZERO, GLenum in_sourceFactor = GL_ONE, GLenum in_operation = GL_ADD);
};

class Shader : public Resource {
public:
	GLuint shadertype;
	GLuint vertexArrayID;

	Shader	(GLuint shader, string path, string name);
	~Shader	();
};

class ShaderProgram : public Resource {
public:
	static ShaderProgram* active;
	GLuint	vertexArrayID;

			ShaderProgram	(string name, Shader* vshader, Shader* fshader, Shader* gshader, Shader* tshader);
			~ShaderProgram	();
	void	Activate		();

	static void Activate	(string name);
	static void Deactivate	();
};

#endif