#version 400

uniform mat4 matrix;

layout(location = 0) in vec2 position_v;
layout(location = 3) in vec3 uv_v;

out vec3 uv_f;

void main() {
	gl_Position = matrix * vec4(position_v, 0.0, 1.0);
	uv_f = uv_v;
}