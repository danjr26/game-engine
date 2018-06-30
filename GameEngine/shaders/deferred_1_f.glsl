#version 400

uniform sampler2D colorTexture;
uniform vec4 colorTint;
in vec3 position_f;
in vec3 normal_f;
in vec4 color_f;
in vec2 uv_f;

layout (location = 0) out vec4 color_o;
layout (location = 1) out vec4 normal_o;

void main() {
	vec2 uv = (uv_f - vec2(0.5)) * 2.0;
	uv = cos(uv * 3.14);
	uv = (uv / 2.0) + vec2(0.5);
	
	color_o = texture(colorTexture, uv_f);
	normal_o = vec4(normal_f, 1.0);
}