#ifndef SHADER_PROGRAM_INSTANCE_H
#define SHADER_PROGRAM_INSTANCE_H

#include "shader_program.h"
#include <unordered_map>

class ShaderProgramInstance {
public:
	enum class UniformType {
		_float, vec2f, vec3f, vec4f,
		_double, vec2d, vec3d, vec4d,
		_int, vec2i, vec3i, vec4i,
		_uint, vec2u, vec3u, vec4u,
		mat2f, mat3f, mat4f,
		mat2x3f, mat2x4f,
		mat3x2f, mat3x4f,
		mat4x2f, mat4x3f,
		mat2d, mat3d, mat4d,
		mat2x3d, mat2x4d,
		mat3x2d, mat3x4d,
		mat4x2d, mat4x3d,
		sampler1D, sampler2D, sampler3D
	};

private:
	struct UniformData {
		UniformType mType;
		GLint mLocation;
		const void* mData;
	};

	ShaderProgram* mProgram;
	std::unordered_map<std::string, UniformData> mUniforms;
	using UniformNameValuePair = std::unordered_map<std::string, UniformData>::value_type;

public:
	ShaderProgramInstance(ShaderProgram* in_program);
	void Assign_Uniform(UniformType in_type, const std::string& in_name, const void* in_value);
	void Remove_Uniform(const std::string& in_name);
	void Use();
};

#endif