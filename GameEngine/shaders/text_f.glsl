#version 400

uniform sampler2DArray colorTextures;
uniform vec2 cullLow;
uniform vec2 cullHigh;

in vec2 position_f;
in vec3 uv_f;

out vec4 color_o;

void main() {
	if (position_f.x < cullLow.x ||
		position_f.x > cullHigh.x ||
		position_f.y < cullLow.y ||
		position_f.y > cullHigh.y) {
			discard;
	}
	color_o = texture(colorTextures, uv_f);
	color_o = vec4(1.0, 1.0, 1.0, pow(color_o.a, 1.0 / 2.2));
}