#version 400

uniform sampler2D 	tex;
uniform int 		texbnd;

in vec4 pass_col;
in vec2 pass_txc;

out vec4 out_col;

void main() {
	out_col = pass_col;
	if(texbnd != 0)
		out_col *= texture(tex, pass_txc);
	//out_col = vec4(1.0);
}