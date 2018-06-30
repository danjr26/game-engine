#version 400

uniform mat4 viematrix;
uniform mat4 modmatrix;

layout(location = 0) in vec3 in_pos;
layout(location = 2) in vec4 in_col;
layout(location = 3) in vec2 in_txc;
layout(location = 4) in float in_wid;

out vec4 pass_col;
out vec2 pass_txc;
out float pass_wid;

void main() {
	mat4 mvmatrix = viematrix * modmatrix;
	gl_Position = mvmatrix * vec4(in_pos, 1.0);
	
	pass_col = in_col;
	pass_txc = in_txc;
	pass_wid = in_wid;
}