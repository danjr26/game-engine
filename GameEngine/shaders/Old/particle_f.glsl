#version 400

uniform sampler2D tex;

in vec4 pass_col_2;
in vec2 pass_txc;

out vec4 out_col;

void main() {
	out_col = texture(tex, pass_txc) * pass_col_2;
	//out_col = vec4(1.0);
}