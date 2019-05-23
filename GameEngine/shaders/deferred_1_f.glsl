#version 400

uniform sampler2D colorTexture;
uniform vec4 color;
in vec3 position_f;
in vec3 normal_f;
in vec4 color_f;
in vec2 uv_f;

layout (location = 0) out vec4 color_o;
layout (location = 1) out vec4 normal_o;

void main() {
	color_o = texture(colorTexture, uv_f) * color;
	normal_o = vec4(normal_f, 1.0);
}
