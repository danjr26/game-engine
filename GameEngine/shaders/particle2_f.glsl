#version 400

uniform sampler2D colorTexture;

in vec4 color_f;
in vec2 uv_f;

out vec4 color_o;

void main() {
	color_o = texture(colorTexture, uv_f) * color_f;
	//color_o = vec4(uv_f.xy, 0.0, 1.0);
	color_o = vec4(1.0);
}
