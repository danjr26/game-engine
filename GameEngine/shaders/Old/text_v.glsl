#version 400

uniform mat4 matrix;

layout(location = 0) in vec3 in_pos;
layout(location = 2) in vec4 in_col;
layout(location = 3) in vec3 in_txc;

out vec3 pass_txc;
out vec4 pass_col;

void main() {
	gl_Position = matrix * vec4(in_pos, 1.0);
	gl_Position.y = -gl_Position.y;
	gl_Position.x /= 400.0;
	gl_Position.y /= 300.0;
	gl_Position.x -= 1.0;
	gl_Position.y += 1.0;
	pass_col = in_col;
	pass_txc = in_txc;
}