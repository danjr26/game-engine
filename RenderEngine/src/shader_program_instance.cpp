#include "shader_program_instance.h"

ShaderProgramInstance::ShaderProgramInstance(ShaderProgram* i_program) :
	mProgram(i_program) {
	
	if (i_program == nullptr) {
		throw InvalidArgumentException("shader program was null");
	}
}

void ShaderProgramInstance::assignUniform(UniformType i_type, const std::string& i_name, const void* i_value) {
	UniformData data = { i_type, mProgram->getUniformLocation(i_name), i_value };
	mUniforms.insert_or_assign(i_name, data);
}

void ShaderProgramInstance::removeUniform(const std::string& i_name) {
	mUniforms.erase(i_name);
}

void ShaderProgramInstance::use() {
	mProgram->use();
	for (auto iter = mUniforms.begin(); iter != mUniforms.end(); iter++) {
		UniformData& uniform = iter->second;
		switch (uniform.mType) {
		case UniformType::_float:	glUniform1fv(uniform.mLocation, 1, (GLfloat*)uniform.mData); break;
		case UniformType::vec2f:	glUniform2fv(uniform.mLocation, 1, (GLfloat*)uniform.mData); break;
		case UniformType::vec3f:	glUniform3fv(uniform.mLocation, 1, (GLfloat*)uniform.mData); break;
		case UniformType::vec4f:	glUniform4fv(uniform.mLocation, 1, (GLfloat*)uniform.mData); break;
		case UniformType::_double:	glUniform1dv(uniform.mLocation, 1, (GLdouble*)uniform.mData); break;
		case UniformType::vec2d:	glUniform2dv(uniform.mLocation, 1, (GLdouble*)uniform.mData); break;
		case UniformType::vec3d:	glUniform3dv(uniform.mLocation, 1, (GLdouble*)uniform.mData); break;
		case UniformType::vec4d:	glUniform4dv(uniform.mLocation, 1, (GLdouble*)uniform.mData); break;
		case UniformType::_int:		glUniform1iv(uniform.mLocation, 1, (GLint*)uniform.mData); break;
		case UniformType::vec2i:	glUniform2iv(uniform.mLocation, 1, (GLint*)uniform.mData); break;
		case UniformType::vec3i:	glUniform3iv(uniform.mLocation, 1, (GLint*)uniform.mData); break;
		case UniformType::vec4i:	glUniform4iv(uniform.mLocation, 1, (GLint*)uniform.mData); break;
		case UniformType::sampler1D:
		case UniformType::sampler2D:
		case UniformType::sampler3D:
		case UniformType::_uint:	glUniform1uiv(uniform.mLocation, 1, (GLuint*)uniform.mData); break;
		case UniformType::vec2u:	glUniform2uiv(uniform.mLocation, 1, (GLuint*)uniform.mData); break;
		case UniformType::vec3u:	glUniform3uiv(uniform.mLocation, 1, (GLuint*)uniform.mData); break;
		case UniformType::vec4u:	glUniform4uiv(uniform.mLocation, 1, (GLuint*)uniform.mData); break;
		case UniformType::mat2f:	glUniformMatrix2fv(uniform.mLocation, 1, GL_TRUE, (GLfloat*)uniform.mData); break;
		case UniformType::mat3f:	glUniformMatrix3fv(uniform.mLocation, 1, GL_TRUE, (GLfloat*)uniform.mData); break;
		case UniformType::mat4f:	glUniformMatrix4fv(uniform.mLocation, 1, GL_TRUE, (GLfloat*)uniform.mData); break;
		case UniformType::mat2x3f:	glUniformMatrix2x3fv(uniform.mLocation, 1, GL_TRUE, (GLfloat*)uniform.mData); break;
		case UniformType::mat2x4f:	glUniformMatrix2x4fv(uniform.mLocation, 1, GL_TRUE, (GLfloat*)uniform.mData); break;
		case UniformType::mat3x2f:	glUniformMatrix3x2fv(uniform.mLocation, 1, GL_TRUE, (GLfloat*)uniform.mData); break;
		case UniformType::mat3x4f:	glUniformMatrix3x4fv(uniform.mLocation, 1, GL_TRUE, (GLfloat*)uniform.mData); break;
		case UniformType::mat4x2f:	glUniformMatrix4x2fv(uniform.mLocation, 1, GL_TRUE, (GLfloat*)uniform.mData); break;
		case UniformType::mat4x3f:	glUniformMatrix4x3fv(uniform.mLocation, 1, GL_TRUE, (GLfloat*)uniform.mData); break;
		case UniformType::mat2d:	glUniformMatrix2dv(uniform.mLocation, 1, GL_TRUE, (GLdouble*)uniform.mData); break;
		case UniformType::mat3d:	glUniformMatrix3dv(uniform.mLocation, 1, GL_TRUE, (GLdouble*)uniform.mData); break;
		case UniformType::mat4d:	glUniformMatrix4dv(uniform.mLocation, 1, GL_TRUE, (GLdouble*)uniform.mData); break;
		case UniformType::mat2x3d:	glUniformMatrix2x3dv(uniform.mLocation, 1, GL_TRUE, (GLdouble*)uniform.mData); break;
		case UniformType::mat2x4d:	glUniformMatrix2x4dv(uniform.mLocation, 1, GL_TRUE, (GLdouble*)uniform.mData); break;
		case UniformType::mat3x2d:	glUniformMatrix3x2dv(uniform.mLocation, 1, GL_TRUE, (GLdouble*)uniform.mData); break;
		case UniformType::mat3x4d:	glUniformMatrix3x4dv(uniform.mLocation, 1, GL_TRUE, (GLdouble*)uniform.mData); break;
		case UniformType::mat4x2d:	glUniformMatrix4x2dv(uniform.mLocation, 1, GL_TRUE, (GLdouble*)uniform.mData); break;
		case UniformType::mat4x3d:	glUniformMatrix4x3dv(uniform.mLocation, 1, GL_TRUE, (GLdouble*)uniform.mData); break;
		default: throw InvalidArgumentException("invalid uniform type");
		}
	}
}
	