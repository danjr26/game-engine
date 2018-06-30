#version 400

uniform sampler2D tex;

in vec2 pass_txc;
in vec3 pass_pos;

out vec4 out_col;

void main() {
	vec2 txc;
	vec2 fromcenter = (pass_txc - vec2(0.5, 0.5)) * 2.0;
	float d = length(fromcenter);
	fromcenter = normalize(fromcenter) * d;
	txc = fromcenter / 2.0 + vec2(0.5, 0.5);
	out_col = 
		texture(tex, txc);
}