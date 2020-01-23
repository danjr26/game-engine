#ifndef SHADER_H
#define SHADER_H

#include "enum_wrappers.h"
#include <GL\glew.h>

class Shader {
friend class ShaderProgram;
private:
	GLuint mId;
	ShaderType mType;

public:
	Shader();
	~Shader();

	void init(ShaderType i_type, const std::string* i_texts, uint i_nTexts);
	void initFromFile(ShaderType i_type, const std::string& i_filename);
	void destroy();
	bool isValid() const;

	ShaderType getType() const;
};

#endif