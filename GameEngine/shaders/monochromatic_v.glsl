#version 400

uniform mat4 mvpMatrix;

layout (location = 0) in vec3 position_v;
layout (location = 2) in vec4 color_v;

out vec4 color_f;

void main() {
	gl_Position = mvpMatrix * vec4(position_v, 1.0);
	color_f = color_v;
}