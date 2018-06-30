#version 400

uniform mat4 viematrix;
uniform mat4 modmatrix;

layout(location = 0) in vec3 in_pos;
layout(location = 2) in vec4 in_col;
layout(location = 3) in vec2 in_txc1;
layout(location = 4) in vec2 in_txc2;
layout(location = 5) in vec3 in_rig;
layout(location = 6) in vec3 in_up;

out vec3 pass_rig;
out vec3 pass_up;
out vec4 pass_col;
out vec2 pass_txc1;
out vec2 pass_txc2;

void main() {
	mat4 mvmatrix = viematrix * modmatrix;
	gl_Position = mvmatrix * vec4(in_pos, 1.0);
	pass_rig = (mvmatrix * vec4(in_rig, 0.0)).xyz;
	pass_up =  (mvmatrix * vec4(in_up, 0.0)).xyz;
	pass_col =  in_col;
	pass_txc1 = in_txc1;
	pass_txc2 = in_txc2;
}

