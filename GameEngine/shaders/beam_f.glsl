#version 400

uniform sampler2D tex;

in vec4 pass_col_2;
in vec2 pass_txc_2;

out vec4 out_col;

void main() {
	out_col = texture(tex, pass_txc_2) * pass_col_2;
	//float m = max();
	
	//out_col = vec4(1.0);
}