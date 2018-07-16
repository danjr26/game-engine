#version 400

uniform mat4 mvpMatrix;

layout (location = 0) in vec3 position_v;
layout (location = 3) in vec2 uv_v;

out vec2 uv_f;

void main() {
	gl_Position = mvpMatrix * vec4(position_v, 1.0);
	uv_f = uv_v;
}

