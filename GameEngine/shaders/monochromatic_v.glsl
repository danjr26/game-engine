#version 400

uniform mat4 mvpMatrix;

layout (location = 0) in vec3 position_v;

void main() {
	gl_Position = mvpMatrix * vec4(position_v, 1.0);
}