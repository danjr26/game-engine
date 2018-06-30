#version 400

uniform sampler2DArray colorTextures;

in vec3 uv_f;

out vec4 color_o;

void main() {
	color_o = texture(colorTextures, uv_f);
	color_o = vec4(1.0, 1.0, 1.0, pow(color_o.a, 1.0 / 2.2));
}