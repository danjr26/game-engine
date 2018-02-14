#version 400

uniform mat4 viematrix;
uniform mat4 promatrix;

layout(location = 0) in vec3 in_pos;

out vec3 pass_pos;

void main() {
	mat4 vpmatrix = promatrix * mat4(mat3(viematrix));
	pass_pos = in_pos;
	gl_Position = vpmatrix * vec4(in_pos, 1.0);
};