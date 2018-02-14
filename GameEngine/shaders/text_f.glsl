#version 400

uniform sampler2DArray texes;

in vec3 pass_txc;
in vec4 pass_col;

out vec4 out_col;

void main() {
	if(pass_txc.x < 0.0 || pass_txc.y < 0.0 || pass_txc.x > 1.0 || pass_txc.y > 1.0)
		out_col = vec4(0.5);
	else
		out_col = texture(texes, pass_txc);
}