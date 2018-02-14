#version 400

layout(location = 0) in vec3 in_pos;
layout(location = 3) in vec2 in_txc;

out vec2 pass_txc;
out vec3 pass_pos;

void main() {
	vec3 pos = in_pos;

	gl_Position = vec4(pos, 1.0);
	pass_txc = in_txc;
	pass_pos = pos;
}