#include "shader_program_instance.h"

ShaderProgramInstance::ShaderProgramInstance(ShaderProgram* in_program) :
	program(in_program) {
	
	if (in_program == nullptr) {
		throw InvalidArgumentException("shader program was null");
	}
}

void ShaderProgramInstance::Assign_Uniform(UniformType in_type, const std::string& in_name, const void* in_value) {
	UniformData data = { in_type, program->Get_Uniform_Location(in_name), in_value };
	uniforms.insert_or_assign(in_name, data);
}

void ShaderProgramInstance::Remove_Uniform(const std::string& in_name) {
	uniforms.erase(in_name);
}

void ShaderProgramInstance::Use() {
	program->Use();
	for (auto iter = uniforms.begin(); iter != uniforms.end(); iter++) {
		UniformData& uniform = iter->second;
		switch (uniform.type) {
		case UniformType::_float:	glUniform1fv(uniform.location, 1, (GLfloat*)uniform.data); break;
		case UniformType::vec2f:	glUniform2fv(uniform.location, 1, (GLfloat*)uniform.data); break;
		case UniformType::vec3f:	glUniform3fv(uniform.location, 1, (GLfloat*)uniform.data); break;
		case UniformType::vec4f:	glUniform4fv(uniform.location, 1, (GLfloat*)uniform.data); break;
		case UniformType::_double:	glUniform1dv(uniform.location, 1, (GLdouble*)uniform.data); break;
		case UniformType::vec2d:	glUniform2dv(uniform.location, 1, (GLdouble*)uniform.data); break;
		case UniformType::vec3d:	glUniform3dv(uniform.location, 1, (GLdouble*)uniform.data); break;
		case UniformType::vec4d:	glUniform4dv(uniform.location, 1, (GLdouble*)uniform.data); break;
		case UniformType::_int:		glUniform1iv(uniform.location, 1, (GLint*)uniform.data); break;
		case UniformType::vec2i:	glUniform2iv(uniform.location, 1, (GLint*)uniform.data); break;
		case UniformType::vec3i:	glUniform3iv(uniform.location, 1, (GLint*)uniform.data); break;
		case UniformType::vec4i:	glUniform4iv(uniform.location, 1, (GLint*)uniform.data); break;
		case UniformType::sampler1D:
		case UniformType::sampler2D:
		case UniformType::sampler3D:
		case UniformType::_uint:	glUniform1uiv(uniform.location, 1, (GLuint*)uniform.data); break;
		case UniformType::vec2u:	glUniform2uiv(uniform.location, 1, (GLuint*)uniform.data); break;
		case UniformType::vec3u:	glUniform3uiv(uniform.location, 1, (GLuint*)uniform.data); break;
		case UniformType::vec4u:	glUniform4uiv(uniform.location, 1, (GLuint*)uniform.data); break;
		case UniformType::mat2f:	glUniformMatrix2fv(uniform.location, 1, GL_TRUE, (GLfloat*)uniform.data); break;
		case UniformType::mat3f:	glUniformMatrix3fv(uniform.location, 1, GL_TRUE, (GLfloat*)uniform.data); break;
		case UniformType::mat4f:	glUniformMatrix4fv(uniform.location, 1, GL_TRUE, (GLfloat*)uniform.data); break;
		case UniformType::mat2x3f:	glUniformMatrix2x3fv(uniform.location, 1, GL_TRUE, (GLfloat*)uniform.data); break;
		case UniformType::mat2x4f:	glUniformMatrix2x4fv(uniform.location, 1, GL_TRUE, (GLfloat*)uniform.data); break;
		case UniformType::mat3x2f:	glUniformMatrix3x2fv(uniform.location, 1, GL_TRUE, (GLfloat*)uniform.data); break;
		case UniformType::mat3x4f:	glUniformMatrix3x4fv(uniform.location, 1, GL_TRUE, (GLfloat*)uniform.data); break;
		case UniformType::mat4x2f:	glUniformMatrix4x2fv(uniform.location, 1, GL_TRUE, (GLfloat*)uniform.data); break;
		case UniformType::mat4x3f:	glUniformMatrix4x3fv(uniform.location, 1, GL_TRUE, (GLfloat*)uniform.data); break;
		case UniformType::mat2d:	glUniformMatrix2dv(uniform.location, 1, GL_TRUE, (GLdouble*)uniform.data); break;
		case UniformType::mat3d:	glUniformMatrix3dv(uniform.location, 1, GL_TRUE, (GLdouble*)uniform.data); break;
		case UniformType::mat4d:	glUniformMatrix4dv(uniform.location, 1, GL_TRUE, (GLdouble*)uniform.data); break;
		case UniformType::mat2x3d:	glUniformMatrix2x3dv(uniform.location, 1, GL_TRUE, (GLdouble*)uniform.data); break;
		case UniformType::mat2x4d:	glUniformMatrix2x4dv(uniform.location, 1, GL_TRUE, (GLdouble*)uniform.data); break;
		case UniformType::mat3x2d:	glUniformMatrix3x2dv(uniform.location, 1, GL_TRUE, (GLdouble*)uniform.data); break;
		case UniformType::mat3x4d:	glUniformMatrix3x4dv(uniform.location, 1, GL_TRUE, (GLdouble*)uniform.data); break;
		case UniformType::mat4x2d:	glUniformMatrix4x2dv(uniform.location, 1, GL_TRUE, (GLdouble*)uniform.data); break;
		case UniformType::mat4x3d:	glUniformMatrix4x3dv(uniform.location, 1, GL_TRUE, (GLdouble*)uniform.data); break;
		default: throw InvalidArgumentException("invalid uniform type");
		}
	}
}
	